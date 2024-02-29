//
// Created by Yinke7 on 2021/12/7.
//

#include "app_flash.h"
#include "stm32f1xx_hal.h"
#include "string.h"

void Flash_Write(uint32_t addr, uint16_t *buff, uint32_t lenth) {

    FLASH_EraseInitTypeDef EraseSet;
    HAL_StatusTypeDef Status;
    uint32_t PageErr;

    EraseSet.TypeErase = FLASH_TYPEERASE_PAGES;
    EraseSet.PageAddress = addr;
    EraseSet.NbPages = 1;
    EraseSet.Banks = FLASH_BANK_1;

    if (lenth > 2048)      //超每页的大小
    {
        printf("Flash program lenth out of range");
        return;
    }

    //擦除
    HAL_FLASH_Unlock();
    Status = HAL_FLASHEx_Erase(&EraseSet, &PageErr);
    HAL_FLASH_Lock();

    if (Status != HAL_OK) {
        printf("Erase fail, PageErr = %lu\r\n", PageErr);
        return;
    }

    //写入
    HAL_FLASH_Unlock();
    for (uint32_t i = 0; i < lenth; i++) {

        Status = HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, addr + i * 2, buff[i]);
        if (Status != HAL_OK) {
            printf("Program fail\r\n");
            return;
        }
    }
    HAL_FLASH_Lock();


}


void Flash_Read(uint32_t addr, uint16_t *buff, uint32_t lenth) {
    for (uint32_t i = 0; i < lenth; i++) {
        buff[i] = *(uint16_t *) (addr + i * 2);
    }
}
