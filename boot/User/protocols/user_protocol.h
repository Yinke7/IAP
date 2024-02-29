/*
 * user_debug.h
 *
 *  Created on: 2022-1-19
 *      Author: Yinke7
 *
 *      此文件用于：
 *      1、定义协议帧结构PT_FRAME_s
 *      2、定义基于协议帧的任务PT_APPLICATION_s
 *      3、底层数据流的接收、帧解析、帧校验
 *      4、环形接收buff的管理
 */

#ifndef USER_DEBUG_H_
#define USER_DEBUG_H_

#include "stm32f1xx_hal.h"
#include "stdbool.h"
#include "string.h"

#if 0
#define PT_VERSION				"V01.01"	//2022.11.11.17.31
//搭建架构
//新增，环形buff，用于接收缓存数据
//读写可异步进行
//提供已封装的协议上层应用结构体PT_APPLICATION_s
//应用程序需自行初始化成员变量，并且根据协议具体字段定义自行编写命令处理逻辑
#else
#define PT_VERSION                "V01.02.01"    //2022.11.15.16.12
//优化，PT_Transport_Alaysis函数：读指针更新逻辑，采用取模的方式，更加便捷准确
//修复，该问题曾导致在数据量过大时，由于PT_Transport_Alaysis和PT_Transport_Recv两个函数都对lenth_written做访问，可能会产生互斥；增加互斥锁解决
//新增，在PT_APPLICATION_s中增加互斥锁，任意时刻，只会被一个操作访问

#endif


#define PT_USE_LOG                0        //调试打印log
#define PT_ANALYSIS_CHECKSUM    0        //在解析数据帧时，需要检验检验和

#define PT_FRAME_HEADER            0x5A    //帧头
#define PT_FRAME_LENTH            20        //帧长度
#define PT_DATA_LENTH_MAX        14        //负载数据最大长度


//数据帧结构
typedef struct {
    unsigned char Header;                        //帧头(0xA5)
    unsigned char Lenth;                      //帧长度(20字节)
    unsigned char Count;                        //下发序号(向上计数，计满清零)
    unsigned char Command;                    //命令码(1-255)
    unsigned char Data[PT_DATA_LENTH_MAX];    //数据(负载)
    unsigned char Checksum[2];                //校验和(0-17Byte的累加和的低两字节，低字节在前)
} PT_FRAME_s;

//应用
typedef struct {
    bool lock;                //互斥锁
    unsigned char *pbuff;            //缓冲区内存，可以指向一个数组
    unsigned int buff_size;        //缓冲区大小
    unsigned int index_write;    //写位置，标记下次写入的位置
    unsigned int index_read;    //读位置，标志下次读取的位置
    unsigned int lenth_written;    //长度，已经写入的数据长度
    PT_FRAME_s frame_send;    //发送帧
    PT_FRAME_s frame_recv;    //接收帧
    void (*pfunc_command_send)(unsigned char cmd, unsigned char *buff, unsigned char lenth);    //发送命令的回调函数，应用层程序对发送数据的处理方式可能不一致，根据需要定义并调用各自处理函数
    void (*pfunc_command_recv)(void);                                        //接收命令的回调函数，应用层程序对接收数据的处理方式可能不一致，根据需要定义并调用各自处理函数
    void (*pfunc_report_send)(unsigned char cmd, unsigned char *buff, unsigned char lenth);    //发送状态的回调函数，应用层程序对发送数据的处理方式可能不一致，根据需要定义并调用各自处理函数
    void (*pfunc_report_recv)(void);                                        //接收报告的回调函数，应用层程序对接收数据的处理方式可能不一致，根据需要定义并调用各自处理函数
} PT_APPLICATION_s;


void PT_CheckSum(PT_FRAME_s *pframe, unsigned char *output);

bool PT_Transport_Recv(PT_APPLICATION_s *papp, unsigned char *buff, unsigned int lenth);

bool PT_Transport_Alaysis(PT_APPLICATION_s *papp);


#endif /* USER_DEBUG_H_ */
