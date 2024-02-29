/*
 * user_debug.h
 *
 *  Created on: 2022-1-19
 *      Author: Yinke7
 *
 *      ���ļ����ڣ�
 *      1������Э��֡�ṹPT_FRAME_s
 *      2���������Э��֡������PT_APPLICATION_s
 *      3���ײ��������Ľ��ա�֡������֡У��
 *      4�����ν���buff�Ĺ���
 */

#ifndef USER_DEBUG_H_
#define USER_DEBUG_H_

#include "stm32f1xx_hal.h"
#include "stdbool.h"
#include "string.h"

#if 0
#define PT_VERSION				"V01.01"	//2022.11.11.17.31
//��ܹ�
//����������buff�����ڽ��ջ�������
//��д���첽����
//�ṩ�ѷ�װ��Э���ϲ�Ӧ�ýṹ��PT_APPLICATION_s
//Ӧ�ó��������г�ʼ����Ա���������Ҹ���Э������ֶζ������б�д������߼�
#else
#define PT_VERSION                "V01.02.01"    //2022.11.15.16.12
//�Ż���PT_Transport_Alaysis��������ָ������߼�������ȡģ�ķ�ʽ�����ӱ��׼ȷ
//�޸���������������������������ʱ������PT_Transport_Alaysis��PT_Transport_Recv������������lenth_written�����ʣ����ܻ�������⣻���ӻ��������
//��������PT_APPLICATION_s�����ӻ�����������ʱ�̣�ֻ�ᱻһ����������

#endif


#define PT_USE_LOG                0        //���Դ�ӡlog
#define PT_ANALYSIS_CHECKSUM    0        //�ڽ�������֡ʱ����Ҫ��������

#define PT_FRAME_HEADER            0x5A    //֡ͷ
#define PT_FRAME_LENTH            20        //֡����
#define PT_DATA_LENTH_MAX        14        //����������󳤶�


//����֡�ṹ
typedef struct {
    unsigned char Header;                        //֡ͷ(0xA5)
    unsigned char Lenth;                      //֡����(20�ֽ�)
    unsigned char Count;                        //�·����(���ϼ�������������)
    unsigned char Command;                    //������(1-255)
    unsigned char Data[PT_DATA_LENTH_MAX];    //����(����)
    unsigned char Checksum[2];                //У���(0-17Byte���ۼӺ͵ĵ����ֽڣ����ֽ���ǰ)
} PT_FRAME_s;

//Ӧ��
typedef struct {
    bool lock;                //������
    unsigned char *pbuff;            //�������ڴ棬����ָ��һ������
    unsigned int buff_size;        //��������С
    unsigned int index_write;    //дλ�ã�����´�д���λ��
    unsigned int index_read;    //��λ�ã���־�´ζ�ȡ��λ��
    unsigned int lenth_written;    //���ȣ��Ѿ�д������ݳ���
    PT_FRAME_s frame_send;    //����֡
    PT_FRAME_s frame_recv;    //����֡
    void (*pfunc_command_send)(unsigned char cmd, unsigned char *buff, unsigned char lenth);    //��������Ļص�������Ӧ�ò����Է������ݵĴ���ʽ���ܲ�һ�£�������Ҫ���岢���ø��Դ�����
    void (*pfunc_command_recv)(void);                                        //��������Ļص�������Ӧ�ò����Խ������ݵĴ���ʽ���ܲ�һ�£�������Ҫ���岢���ø��Դ�����
    void (*pfunc_report_send)(unsigned char cmd, unsigned char *buff, unsigned char lenth);    //����״̬�Ļص�������Ӧ�ò����Է������ݵĴ���ʽ���ܲ�һ�£�������Ҫ���岢���ø��Դ�����
    void (*pfunc_report_recv)(void);                                        //���ձ���Ļص�������Ӧ�ò����Խ������ݵĴ���ʽ���ܲ�һ�£�������Ҫ���岢���ø��Դ�����
} PT_APPLICATION_s;


void PT_CheckSum(PT_FRAME_s *pframe, unsigned char *output);

bool PT_Transport_Recv(PT_APPLICATION_s *papp, unsigned char *buff, unsigned int lenth);

bool PT_Transport_Alaysis(PT_APPLICATION_s *papp);


#endif /* USER_DEBUG_H_ */
