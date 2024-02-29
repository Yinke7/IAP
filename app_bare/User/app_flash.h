//
// Created by Yinke7 on 2021/12/7.
//

#ifndef XPT2108_FTE_TX_APP_FLASH_H
#define XPT2108_FTE_TX_APP_FLASH_H

#include "user_config.h"

void Flash_Read(uint32_t addr, uint16_t *buff, uint32_t lenth);

void Flash_Write(uint32_t addr, uint16_t *buff, uint32_t lenth);


#endif //XPT2108_FTE_TX_APP_FLASH_H
