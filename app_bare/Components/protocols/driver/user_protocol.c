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
 * ������״̬
 * ����papp��ָ����Ҫ��������Ӧ��
 * ����en�������Ƿ�����
 * */
static void PT_SetLock(PT_APPLICATION_s * papp, bool en)
{
	if(papp->lock != en)
	{
		papp->lock = en;
	}
}

/*
 * ��ȡ��״̬
 * ����papp��ָ����Ҫ��ȡ����Ӧ�á�
 * ����ֵ��������״̬
 * */
static bool PT_GetLock(PT_APPLICATION_s * papp)
{
	return papp->lock;
}



/**
 * ֡У��
 * ����pframe����Ҫ����У��͵�֡
 * ����output��ָ����У��ͽ�����ڴ�
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
 * ���ܣ����յײ����������ֻҪ�������Ŀ���ռ������ɴ˴������������ɽ��뻺��������֮������Դ˴�����������ͨ������Ӧ�õĻ��������������ٴ����������
 * �÷���ÿ������������Ϳɵ��ã�������ڴ����жϷ������У����ڽ����ж�һ�����������������ã�
 * ����papp��ָ��ʹ�ø���������Ӧ��
 * ����buff��ָ��˴ε��������ڴ���ʼ��ַ
 * ����lenth���˴��������ĳ���
 * ����ֵ���ɹ����˴�������д�뻺��������true����֮����false
 */
bool PT_Transport_Recv(PT_APPLICATION_s * papp, unsigned char *buff, unsigned int lenth)
{
	//�����״̬
	if(PT_GetLock(papp) == true)
	{
		return false;
	}

	//����
	PT_SetLock(papp, true);

	bool res = false;

	//���ݻ�������ǰ����������
	//�����ִ���ʽ��1���������ݣ�2����ȡ������������������������
	//������÷�ʽ1�������ɸ������󲹳䷽ʽ2
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
		if(papp->index_write >= papp->index_read)		//дָ���ڶ�ָ�����
		{
			if(papp->index_write + lenth <= papp->buff_size)
			{
				memcpy(papp->pbuff + papp->index_write, buff, lenth);
			}
			else 	//�ֶν���
			{
				unsigned int len_head = papp->buff_size - papp->index_write;	//ͷ��
				unsigned int len_tail = lenth - len_head;							//β��
				memcpy(papp->pbuff + papp->index_write, buff, len_head);
				memcpy(papp->pbuff, buff + len_head, len_tail);
			}
		}
		else if(papp->index_write < papp->index_read)		//дָ���ڶ�ָ��ǰ��
		{
			memcpy(papp->pbuff + papp->index_write, buff, lenth);
		}
		papp->index_write = (papp->index_write + lenth) % papp->buff_size;	//����дָ��λ��
		papp->lenth_written += lenth;	//���´���ȡ����

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

	//����
	PT_SetLock(papp, false);
	return res;
}

/*
 * ���ܣ�������Ч֡���ڵ���֮�����۽���ɹ�����ʧ�ܣ��������Ļ���ռ䶼�ᱻ�ͷŵ���
 * �÷����ڳɹ����յ�������֮�󣬲����ڴ�֮����κ�ʱ�̶����Ե��ã������ڻ�������ţ����ᱻ���ǣ�ֱ����������
 * ����papp: ָ����Ҫ���н�����Ӧ��
 * ����ֵ���ɹ���������Ч֡����true����֮������false
 * */
bool PT_Transport_Alaysis(PT_APPLICATION_s *papp)
{
	//�����״̬
	if(PT_GetLock(papp) == true)
	{
		return false;
	}

	//����
	PT_SetLock(papp, true);

	bool res = false;
	bool found = false;
	unsigned int offset = 0;
	unsigned char sz = sizeof(PT_FRAME_s);
	unsigned char chk_sum[2] = {0};
	unsigned char tmp[PT_FRAME_LENTH] = {0};	//��ʱ���棬ֻ����һ֡����
	PT_FRAME_s *pframe;

    //����С��֡��������Ϊ���ݻ�δ�����������������ݳ������Ȳ�������
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

		//���Ȳ�С��֡��
		while(1)
		{
			if(papp->index_read + offset + sz <= papp->buff_size)
			{
				pframe = (PT_FRAME_s *)(papp->pbuff + papp->index_read + offset);
			}
			else	//�ֶζ�ȡ
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


			//����������ȡ����Ѱ������֡
			offset ++;
			if(offset + sz > papp->lenth_written)
			{
				break;
			}
		}

		if(found == true)
		{
			papp->index_read = (papp->index_read + offset + sz) % papp->buff_size;	//���¶�ָ��λ��
			papp->lenth_written -= (offset + sz);	//���´���ȡ����
			res = true;
		}
		else
		{
			papp->index_read = (papp->index_read + offset) % papp->buff_size;		//���¶�ָ��λ��
			papp->lenth_written -= offset;			//���´���ȡ����
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

	//����
	PT_SetLock(papp, false);
	return res;
}
