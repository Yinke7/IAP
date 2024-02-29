/*
 * user_debug.c
 *
 *  Created on: 2022-1-19
 *      Author: Yinke7
 */


#include "user_protocol.h"
#if PT_USE_LOG == 1
#include "../../app_uart.h"
#endif



#define TAG		"user_protocol"

static void PT_SetLock(PT_APPLICATION_s * papp, bool en);
static bool PT_GetLock(PT_APPLICATION_s * papp);

/*
 * 设置锁状态
 * 参数papp：指向需要设置锁的应用
 * 参数en：设置是否上锁
 * */
static void PT_SetLock(PT_APPLICATION_s * papp, bool en)
{
	if(papp->lock != en)
	{
		papp->lock = en;
	}
}

/*
 * 获取锁状态
 * 参数papp：指向需要获取锁的应用、
 * 返回值：返回锁状态
 * */
static bool PT_GetLock(PT_APPLICATION_s * papp)
{
	return papp->lock;
}



/**
 * 帧校验
 * 参数pframe：需要计算校验和的帧
 * 参数output：指向存放校验和结果的内存
 */
void PT_CheckSum(PT_FRAME_s *pframe, unsigned char *output)
{
    unsigned int sum = 0;
    for (unsigned char i = 0; i < 18; i++)
    {
        sum += ((unsigned char *)pframe)[i];
    }

    for (unsigned char i = 0; i < sizeof(pframe->Checksum); i++)
    {
    	output[i] = (sum >> (8 * i)) & 0xff;
    }
}

/**
 * 功能：接收底层的数据流，只要缓冲区的空余空间能容纳此次数据流，都可进入缓冲区，反之，则忽略此次数据流（可通过增大应用的缓冲区长度来减少此情况发生）
 * 用法：每当数据流到达就可调用（比如放在串口中断服务函数中，串口接收中断一旦被触发，立即调用）
 * 参数papp：指向使用该数据流的应用
 * 参数buff：指向此次的数据流内存起始地址
 * 参数lenth：此次数据流的长度
 * 返回值：成功将此次数据流写入缓冲区返回true；反之返回false
 */
bool PT_Transport_Recv(PT_APPLICATION_s * papp, unsigned char *buff, unsigned int lenth)
{
	//检查锁状态
	if(PT_GetLock(papp) == true)
	{
		return false;
	}

	//上锁
	PT_SetLock(papp, true);

	bool res = false;

	//根据缓冲区当前长度做处理
	//有两种处理方式：1、拒收数据；2、截取并接收数据至缓冲区被填满
	//这里采用方式1，后续可更具需求补充方式2
	if(papp->lenth_written + lenth > papp->buff_size)
	{

#if PT_USE_LOG == 1
		{
			char str_p[100] = {0};
			u_sprintf(str_p,"[%s]:err lenth%d pw%d lw%d\n",
					TAG,
					lenth,
					papp->index_write,
					papp->lenth_written
					);
			trans_buff.dma_len = strlen(str_p);
			memcpy(trans_buff.data, str_p, trans_buff.dma_len);
			uart_dma_send((unsigned char*)&trans_buff);
			WaitMs(10);
		}
#endif

		res = false;
	}
	else
	{
		if(papp->index_write >= papp->index_read)		//写指针在读指针后面
		{
			if(papp->index_write + lenth <= papp->buff_size)
			{
				memcpy(papp->pbuff + papp->index_write, buff, lenth);
			}
			else 	//分段接收
			{
				unsigned int len_head = papp->buff_size - papp->index_write;	//头部
				unsigned int len_tail = lenth - len_head;							//尾部
				memcpy(papp->pbuff + papp->index_write, buff, len_head);
				memcpy(papp->pbuff, buff + len_head, len_tail);
			}
		}
		else if(papp->index_write < papp->index_read)		//写指针在读指针前面
		{
			memcpy(papp->pbuff + papp->index_write, buff, lenth);
		}
		papp->index_write = (papp->index_write + lenth) % papp->buff_size;	//更新写指针位置
		papp->lenth_written += lenth;	//更新待读取长度

#if PT_USE_LOG == 1
		{
			char str_p[100] = {0};
			u_sprintf(str_p,"[%s]:ok lenth%d pw%d lw%d\n",
					TAG,
					lenth,
					papp->index_write,
					papp->lenth_written
					);
			trans_buff.dma_len = strlen(str_p);
			memcpy(trans_buff.data, str_p, trans_buff.dma_len);
			uart_dma_send((unsigned char*)&trans_buff);
			WaitMs(10);
		}
#endif

		res = true;
	}

	//解锁
	PT_SetLock(papp, false);
	return res;
}

/*
 * 功能：解析有效帧（在调用之后，无论结果成功还是失败，被解析的缓存空间都会被释放掉）
 * 用法：在成功接收到数据流之后，并且在此之后的任何时刻都可以调用（数据在缓冲区存放，不会被覆盖，直至被解析）
 * 参数papp: 指向需要进行解析的应用
 * 返回值：成功解析出有效帧返回true；反之，返回false
 * */
bool PT_Transport_Alaysis(PT_APPLICATION_s *papp)
{
	//检查锁状态
	if(PT_GetLock(papp) == true)
	{
		return false;
	}

	//上锁
	PT_SetLock(papp, true);

	bool res = false;
	bool found = false;
	unsigned int offset = 0;
	unsigned char sz = sizeof(PT_FRAME_s);
	unsigned char chk_sum[2] = {0};
	unsigned char tmp[PT_FRAME_LENTH] = {0};	//临时缓存，只够放一帧数据
	PT_FRAME_s *pframe;

    //长度小于帧长，则认为数据还未接收完整，或者数据出错，则先不做处理；
	if(papp->lenth_written < sz)
    {
		res = false;
    }
	else
	{
#if PT_USE_LOG == 1
		{
			char str_p[100] = {0};
			u_sprintf(str_p,"[%s]:start oft%d pw%d pr%d lw%d\n",
					TAG,
					offset, papp->index_write, papp->index_read, papp->lenth_written);
			trans_buff.dma_len = strlen(str_p);
			memcpy(trans_buff.data, str_p, trans_buff.dma_len);
			uart_dma_send((unsigned char*)&trans_buff);
			WaitMs(10);
		}
#endif

		//长度不小于帧长
		while(1)
		{
			if(papp->index_read + offset + sz <= papp->buff_size)
			{
				pframe = (PT_FRAME_s *)(papp->pbuff + papp->index_read + offset);
			}
			else	//分段读取
			{
				unsigned int len_head = 0;
				unsigned int len_tail = 0;
				if(papp->index_read + offset <= papp->buff_size)
				{
					len_head = papp->buff_size - (papp->index_read + offset);
					len_tail = sz - len_head;
					memcpy(tmp, papp->pbuff + papp->index_read + offset, len_head);
					memcpy(tmp + len_head, papp->pbuff, len_tail);
				}
				else
				{
					unsigned int len = (papp->index_read + offset) - papp->buff_size;
					memcpy(tmp, papp->pbuff + len, sz);
				}
				pframe = (PT_FRAME_s *)tmp;
			}

#if PT_ANALYSIS_CHECKSUM == 1
			PT_CheckSum(pframe, chk_sum);
#endif
			if (pframe->Header == PT_FRAME_HEADER
#if PT_ANALYSIS_CHECKSUM == 1
					&& memcmp(pframe->Checksum, chk_sum, sizeof(pframe->Checksum)) == 0
#endif
					) {
				memcpy(&papp->frame_recv, pframe, sz);

				found = true;
				break;
			}


			//遍历带待读取区域，寻找完整帧
			offset ++;
			if(offset + sz > papp->lenth_written)
			{
				break;
			}
		}

		if(found == true)
		{
			papp->index_read = (papp->index_read + offset + sz) % papp->buff_size;	//更新读指针位置
			papp->lenth_written -= (offset + sz);	//更新待读取长度
			res = true;
		}
		else
		{
			papp->index_read = (papp->index_read + offset) % papp->buff_size;		//更新读指针位置
			papp->lenth_written -= offset;			//更新待读取长度
			res = false;
		}

#if PT_USE_LOG == 1
		{
			char str_p[100] = {0};
			u_sprintf(str_p,"[%s]:[%d] end oft%d pw%d pr%d lw%d\n",
					TAG,
					res?1:0, offset, papp->index_write, papp->index_read, papp->lenth_written);
			trans_buff.dma_len = strlen(str_p);
			memcpy(trans_buff.data, str_p, trans_buff.dma_len);
			uart_dma_send((unsigned char*)&trans_buff);
			WaitMs(10);
		}
#endif
	}

	//解锁
	PT_SetLock(papp, false);
	return res;
}
