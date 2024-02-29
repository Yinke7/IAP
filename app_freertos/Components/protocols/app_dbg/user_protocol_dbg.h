/*
 * user_protocol_dbg.h
 *
 *  Created on: 2023-2-8
 *      Author: Yinke7
 */

#ifndef USER_PROTOCOL_DBG_H_
#define USER_PROTOCOL_DBG_H_

#include "../driver/user_protocol.h"
/* user code begain*/

/*user code end*/

#if 0
#define PT_DBG_VERSION				"V01.01.01"	//Ӧ�ð汾
#else
#define PT_DBG_VERSION                "V01.02.01"    //2024.01.31.17.04
//���£�����ӻ�ɾ�������Ĳ����ϲ�Ϊһ����������ֲ
//���£��ڶ�Flashʱ����ӷ�Χ������ֻ�ܶ�ȡһ������(4KBytes)



#endif

#define RECV_BUFF_DBG_LENTH_MAX        256        //ͨѶ���ջ�����󳤶�
#define DBG_VERSION_BUFF_LENTH        128        //���ڴ�Ű汾��Ϣ�Ļ��泤��

#define PARA_WRITEFLASH_ENABLE        1        //�Ƿ���Ҫ����Flash
#if PARA_WRITEFLASH_ENABLE
//#error "Set a value for FLASH_ADDR_PARAMS and comment this line"
#define FLASH_ADDR_PARAMS            FLASH_RTC_SET_ADDR    //Flash��ŵ�ַ
#endif
#define DBG_COUNT_MAX            (sizeof (DBG_PARA_GROUP_s)/sizeof (DBG_PARA_ITEM_s))    //�����ܸ���


/**
 * ����������
 * �û��������������������
 * 1.���һ��ķ�ʽ����ĩβ���� X_MACRO(n, v, c, d, w, r, f)
 *      n �������������ı�����
 *      v ����ֵ���ڳ�ʼ�����ô�ֵΪ������ֵ
 *      c �����룺�����������ֵ
 *      d ������һ���ַ���������С�� 8 ���ֽ�
 *      w,r,f ���ԣ�����/��д:w = 0/1;����/�ɶ�:r = 0/1;����/�ɲٷ���Flash:f = 0/1;
 * 2.ɾ��һ��ķ�ʽ��
 *      ɾ������
 *      ���ע�ͣ�ע�Ᵽ����ĩβ��б��'\'
 * 3.�����ԣ�
 *      ����������������� [0, 255] ���������ֵ�����Բ��������������ظ���
 */
#define DBG_TAB \
        X_MACRO(Vi1_L, 111, 0, "Vi1_L", 1, 1, 1)    \
        X_MACRO(Vi1_H, 222, 1, "Vi1_H", 1, 1, 1)    \
        X_MACRO(Vi2_L, 333, 2, "Vi2_L", 1, 1, 1)    \
        X_MACRO(Vi2_H, 444, 5, "Vi2_H", 1, 1, 1)    \
        X_MACRO(Vi3_L, 555, 7, "Vi3_L", 1, 1, 1)    \
        X_MACRO(Vi3_H, 666, 9, "Vi3_H", 1, 1, 1)    \
        /*X_MACRO(n, v, c, d, w, r, f)*/


typedef union {
    struct {
        unsigned char icd: 1;
        unsigned char vcd: 1;
    } m_s;
    unsigned char mark;
} Para_Mark_u;


/** Э������ **/
typedef enum {
    CB_DBG_REQUEST = 100,
    CB_DBG_EXIT,
    CB_GET_VER,
    CB_GET_PARA_COUNT,
    CB_READALL_PARAS,
    CB_PARA_WRITE,
    CB_PARA_READ,
} CMD_BuiltIn_e;


/** ֡����Data[PT_DATA_LENTH_MAX]�ĸ�ʽ��ϸ��Ϊ���ֽṹ **/
typedef union {
    /** �ظ� **/
    struct {
        unsigned char res[PT_DATA_LENTH_MAX];            //����ռ䣬��������ڴ�
    } ver_reply_s;

    /** ���� **/
    struct {
        unsigned char flag;        //���
        unsigned char res[PT_DATA_LENTH_MAX - 1];        //����ռ䣬��������ڴ�
    } request_s;

    /** ������ **/
    struct {
        unsigned char cmd;        //������
        unsigned char value[4];    //����ֵ
        unsigned char desc[8];    //��������
        union {
            struct {
                unsigned char write: 1;
                unsigned char read: 1;
                unsigned char flash: 1;
            } p_s;
            unsigned char properties;
        } prop_s;            //��������
    } para_s;

    /** ��������Ŀ **/
    struct {
        unsigned char count[4];    //����Ŀ
        unsigned char res[PT_DATA_LENTH_MAX - 4];        //����ռ䣬��������ڴ�
    } get_para_count_s;

    /** �汾��Ϣ **/
    struct {
        unsigned char index;
        unsigned char data[PT_DATA_LENTH_MAX - 1];
    } ver_s;

} PT_PACK_DBG_u;


typedef struct {
    unsigned int value;                //ֵ
    unsigned char cmd;                //������
    unsigned char desc[8];            //�������������ļ���ʽһ������ΪGB2312��ʽ������ΪUTF-8������
    union {
        struct {
            unsigned char write: 1;    //0������д��1����д
            unsigned char read: 1;    //0�����ɶ���1���ɶ�
            unsigned char flash: 1;    //0�����ɴ浽Flash��1���ɱ��浽Flash

            unsigned char resv: 5;    //����λ�����ù���
        } p_s;
        unsigned char properties;
    } prop_s;

} DBG_PARA_ITEM_s;


/** ����������  **/
typedef enum {
#define X_MACRO(n, v, c, d, w, r, f)  DBG_##n = c,
    DBG_TAB
#undef X_MACRO
} DBG_CMD_PARAITEM_e;

/** ������  **/
typedef struct {
#define X_MACRO(n, v, c, d, w, r, f)  DBG_PARA_ITEM_s n;
    DBG_TAB
#undef X_MACRO
} DBG_PARA_GROUP_s;

/**����Flashʱ�õ������ݽṹ**/
typedef struct {
    unsigned char cmd;
    unsigned int value;
} DBG_PARA_KEY_VALUE_s;

void DBG_Version_Combine(unsigned char *hw_ver, unsigned char *sw_ver, unsigned char *adv_name);

void DBG_Read_Params_FromFlash(void);

void DBG_Write_Params_ToFlash(void);

unsigned int DBG_Init_Params(void);

extern bool DBG_Flag;
extern unsigned char DBG_Para_Mark[DBG_COUNT_MAX];
extern PT_APPLICATION_s PT_App_DBG;
extern DBG_PARA_GROUP_s ParaGroup;


#endif /* USER_PROTOCOL_DBG_H_ */
