//
// Created by Administrator on 2021/9/3.
//

#include "SPI_software.h"

#if __SPI_USE_SOFTWARE__ == 1

void delay_us(uint32_t nus) {
    uint32_t Delay = nus * 72;
    do {
        __NOP();
    } while (Delay--);
}


UBYTE SPI_Software_Send_Byte(UBYTE write_dat) {
    UBYTE i, read_dat = 0;
    SPI_PIN_CLK_L;
    SPI_PIN_MOSI_L;
    for (i = 0; i < 8; i++) {
        if (write_dat & 0x80)
            SPI_PIN_MOSI_H;
        else
            SPI_PIN_MOSI_L;
        write_dat <<= 1;
//		__NOP();
        SPI_PIN_CLK_H;
        read_dat <<= 1;
//		if (SPI_PIN_MISO_R)
//			read_dat++;
//		__NOP();
        SPI_PIN_CLK_L;
        SPI_PIN_MOSI_L;
    }

    return read_dat;
}

void SPI_Soft_GPIO_Init(void) {

    GPIO_InitTypeDef GPIO_InitStruct = {0};

    /* GPIO Ports Clock Enable */
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();

    HAL_GPIO_WritePin(LCD_RST_GPIO_Port, LCD_RST_Pin, GPIO_PIN_SET);


    GPIO_InitStruct.Pin = LCD_RST_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);


    GPIO_InitStruct.Pin = LCD_DC_Pin | LCD_SCLK_Pin | LCD_MOSI_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /*Configure GPIO pins : PCPin PCPin */
    GPIO_InitStruct.Pin = LCD_MISO_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}

#endif
