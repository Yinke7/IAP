//
// Created by Yinke7 on 2023/12/6.
//

#ifndef BOOTLOADER_APP_UPDATE_H
#define BOOTLOADER_APP_UPDATE_H

#include "stdbool.h"
#include "app_flash.h"


typedef enum {
    U_NORMAL = 0,
    U_REQUEST,              //请求
    U_SATRT,                //升级开始
    U_UPDATTING,            //升级中
    U_END,                  //升级结束
    U_ERR_BREAKOFF,         //升级被中断错误
    U_ERR_CRC,              //升级数据校验错误
    U_ERR_TIMEOUT,          //升级超时
    U_ERR_UNKNOWN,          //未知错误
} UPDATE_STATE_e;


typedef struct {
    uint8_t state;
    uint32_t lenth;
    uint32_t crc32;
} UPDATE_PARA_s;


bool Update_Check_Normal(void);


#endif //BOOTLOADER_APP_UPDATE_H
