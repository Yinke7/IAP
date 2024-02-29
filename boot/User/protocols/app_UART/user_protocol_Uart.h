/*
 * user_protocol_custom.h
 *
 *  Created on: 2022-11-11
 *      Author: Yinke7
 */

#ifndef USER_PROTOCOL_CUSTOM_H_
#define USER_PROTOCOL_CUSTOM_H_

#include "../user_protocol.h"

#if 0
#else
#define PT_DBG_VERSION                          "V01.01.01"    //应用版本
#endif


#define UART_RECV_BUFF_LENTH_MAX    256        //通讯接收缓存最大长度
#define PARA_WRITEFLASH_COUNT_MAX   20         //最多可读参数个数
#define FLASH_ADDR_PARA_SETTTING    (0x08000000+0x3F800)

//指令
typedef enum {
    RC_CMD_RTC_SET = 0, //设置RTC
    CB_DBG_REQUEST = 100,
    CB_DBG_EXIT,
    CB_GET_VER,
    CB_GET_PARA_COUNT,
    CB_READALL_PARAS,
    CB_PARA_WRITE,
    CB_PARA_READ,
    //预留
    //...

    RC_CMD_ERR = 0xFF,    //命令解析失败

} RS485_CMD_e;

/** 参数命令码 **/
typedef enum {
    DBG_IF_5V_L = 0,
    DBG_IF_5V_H,
    DBG_COUNT_MAX,    //计数
} DBG_CMD_PARAITEM_e;

//上报数据帧中，各命令码对应的包结构
typedef union {
    /** 模板 **/
    struct {
        unsigned char res[PT_DATA_LENTH_MAX];
    } tmp_s;

    /** RTC设置 **/
    struct {
        unsigned char data[PT_DATA_LENTH_MAX];
    } rtc_set_s;

    /** 回复 **/
    struct {
        unsigned char res[PT_DATA_LENTH_MAX];            //空余空间，用于填充内存
    } ver_reply_s;

    /** 请求 **/
    struct {
        unsigned char flag;        //标记
        unsigned char res[PT_DATA_LENTH_MAX - 1];        //空余空间，用于填充内存
    } request_s;

    /** 参数项 **/
    struct {
        unsigned char cmd;        //参数码
        unsigned char value[4];    //参数值
        unsigned char desc[8];    //参数描述
        union {
            struct {
                unsigned char write: 1;
                unsigned char read: 1;
                unsigned char flash: 1;
            } p_s;
            unsigned char properties;
        } prop_s;            //参数属性
    } para_s;

    /** 参数总数目 **/
    struct {
        unsigned char count[4];    //总数目
        unsigned char res[PT_DATA_LENTH_MAX - 4];        //空余空间，用于填充内存
    } get_para_count_s;

    /** 版本信息 **/
    struct {
        unsigned char index;
        unsigned char data[PT_DATA_LENTH_MAX - 1];
    } ver_s;

} PT_PACK_UART_u;


typedef struct {
    unsigned int value;
    unsigned char cmd;
    unsigned char desc[8];
    union {
        struct {
            unsigned char write: 1;
            unsigned char read: 1;
            unsigned char flash: 1;
        } p_s;
        unsigned char properties;
    } prop_s;

} DBG_PARA_ITEM_s;

typedef struct {
    DBG_PARA_ITEM_s if_5v_l;
    DBG_PARA_ITEM_s if_5v_h;

} DBG_PARA_GROUP_s;

typedef struct {
    unsigned char cmd;
    unsigned int value;
} DBG_PARA_KEY_VALUE_s;

unsigned int DBG_Init_Params(void);

extern PT_APPLICATION_s PT_App_UART;

extern DBG_PARA_GROUP_s ParaGroup;

#endif /* USER_PROTOCOL_CUSTOM_H_ */
