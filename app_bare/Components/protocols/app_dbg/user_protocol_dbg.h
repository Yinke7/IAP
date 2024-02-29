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
#define PT_DBG_VERSION				"V01.01.01"	//应用版本
#else
#define PT_DBG_VERSION                "V01.02.01"    //2024.01.31.17.04
//更新，将添加或删除参数的操作合并为一处，便于移植
//更新，在读Flash时，添加范围保护，只能读取一个扇区(4KBytes)



#endif

#define RECV_BUFF_DBG_LENTH_MAX        256        //通讯接收缓存最大长度
#define DBG_VERSION_BUFF_LENTH        128        //用于存放版本信息的缓存长度

#define PARA_WRITEFLASH_ENABLE        1        //是否需要操作Flash
#if PARA_WRITEFLASH_ENABLE
//#error "Set a value for FLASH_ADDR_PARAMS and comment this line"
#define FLASH_ADDR_PARAMS            FLASH_RTC_SET_ADDR    //Flash存放地址
#endif
#define DBG_COUNT_MAX            (sizeof (DBG_PARA_GROUP_s)/sizeof (DBG_PARA_ITEM_s))    //参数总个数


/**
 * 参数表设置
 * 用户在这里根据需求做更改
 * 1.添加一项的方式：在末尾加入 X_MACRO(n, v, c, d, w, r, f)
 *      n 参数名：参数的变量名
 *      v 参数值：在初始化中用此值为参数赋值
 *      c 命令码：参数命令码的值
 *      d 描述：一条字符串，长度小于 8 个字节
 *      w,r,f 属性：不可/可写:w = 0/1;不读/可读:r = 0/1;不可/可操访问Flash:f = 0/1;
 * 2.删除一项的方式：
 *      删除整行
 *      添加注释，注意保留行末尾的斜杠'\'
 * 3.新特性：
 *      命令码可以随意设置 [0, 255] 区间的任意值，可以不连续，但不能重复！
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


/** 协议命令 **/
typedef enum {
    CB_DBG_REQUEST = 100,
    CB_DBG_EXIT,
    CB_GET_VER,
    CB_GET_PARA_COUNT,
    CB_READALL_PARAS,
    CB_PARA_WRITE,
    CB_PARA_READ,
} CMD_BuiltIn_e;


/** 帧数据Data[PT_DATA_LENTH_MAX]的格式，细分为多种结构 **/
typedef union {
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

} PT_PACK_DBG_u;


typedef struct {
    unsigned int value;                //值
    unsigned char cmd;                //命令码
    unsigned char desc[8];            //描述，！！！文件格式一定设置为GB2312格式，不能为UTF-8！！！
    union {
        struct {
            unsigned char write: 1;    //0：不可写，1：可写
            unsigned char read: 1;    //0：不可读，1：可读
            unsigned char flash: 1;    //0：不可存到Flash，1：可保存到Flash

            unsigned char resv: 5;    //保留位，不用关心
        } p_s;
        unsigned char properties;
    } prop_s;

} DBG_PARA_ITEM_s;


/** 参数命令码  **/
typedef enum {
#define X_MACRO(n, v, c, d, w, r, f)  DBG_##n = c,
    DBG_TAB
#undef X_MACRO
} DBG_CMD_PARAITEM_e;

/** 参数项  **/
typedef struct {
#define X_MACRO(n, v, c, d, w, r, f)  DBG_PARA_ITEM_s n;
    DBG_TAB
#undef X_MACRO
} DBG_PARA_GROUP_s;

/**访问Flash时用到的数据结构**/
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
