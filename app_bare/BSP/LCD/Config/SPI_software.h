//
// Created by Administrator on 2021/9/3.
//

#ifndef FROCK_SPI_SOFTWARE_H
#define FROCK_SPI_SOFTWARE_H

#include "stm32f1xx_hal.h"
#include "DEV_Config.h"
#include "user_config.h"

#if __SPI_USE_SOFTWARE__ == 1
#define SPI_PIN_CLK_L               HAL_GPIO_WritePin(LCD_SCLK_GPIO_Port, LCD_SCLK_Pin, GPIO_PIN_RESET)
#define SPI_PIN_CLK_H               HAL_GPIO_WritePin(LCD_SCLK_GPIO_Port, LCD_SCLK_Pin, GPIO_PIN_SET)
#define SPI_PIN_MOSI_L              HAL_GPIO_WritePin(LCD_MOSI_GPIO_Port, LCD_MOSI_Pin, GPIO_PIN_RESET)
#define SPI_PIN_MOSI_H              HAL_GPIO_WritePin(LCD_MOSI_GPIO_Port, LCD_MOSI_Pin, GPIO_PIN_SET)
//#define SPI_PIN_MISO_R              HAL_GPIO_ReadPin(SPI_SW_MISO_GPIO_Port, SPI_SW_MISO_Pin)

void delay_us(uint32_t udelay);

UBYTE SPI_Software_Send_Byte(UBYTE data);

void SPI_Soft_GPIO_Init(void);
#else
//null
#endif

#endif //FROCK_SPI_SOFTWARE_H
