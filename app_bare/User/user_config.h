//
// Created by Administrator on 2021/9/1.
//

#ifndef CH376S_YK_CONFIG_H
#define CH376S_YK_CONFIG_H

#include "main.h"
#include "usart.h"
#include "tim.h"
#include "stdio.h"
#include "stdbool.h"

#define VT_12V6                         0
#define VT_16V8                         1
#define VT_18V5                         2

#define ST_DEBUGING                     0
#define ST_TESTTING                     1
#define ST_RELEASED                     2


#define VOLTAGE_OUTPUT_TYPE                     VT_18V5
#define VER_STATUS_TYPE                         ST_DEBUGING

#if VOLTAGE_OUTPUT_TYPE == VT_12V6
#define PRODUCT_NO                              "XPT2108-050L-TX-FT04-FTE_"
#elif VOLTAGE_OUTPUT_TYPE == VT_18V5
#define PRODUCT_NO                              "XPT2108-050L-TX-FT06-FTE_"
#elif VOLTAGE_OUTPUT_TYPE == VT_16V8
#define PRODUCT_NO                              "XPT2108-050L-TX-FT03-FTE_"
#endif

#if VER_STATUS_TYPE == ST_DEBUGING
#define SUFFIX_NO                               ".01"
#elif VER_STATUS_TYPE == ST_TESTTING
#define SUFFIX_NO 						        ".02"
#elif VER_STATUS_TYPE == ST_RELEASED
#define SUFFIX_NO 						        ".03"
#endif

#define HW_VERSION                              "XPT2108-050L-FT01-TX-FTE-V1.0"
/*
 *
 */
#define SW_VERSION                              "SW-TX-V00.01"              //2022.04.06.14.43
//FreeRTOS Demo



#define FLASH_RTC_SET_ADDR                  (0x08000000 + 0x3F800)      //RTC�������

#define __DEBUG_DRIVERS__           0
#define __DEBUG_BREAK_ERR__         1               //0�����Թ������в����ͨ��ʱ�������� 1�����Թ������в����ͨ��ʱ��������

#define __SPI_USE_SOFTWARE__        0               //0��SPI���ģ�� 1��SPIӲ������
#define __CH376S_INT_USE_IO__       0               //0���жϲ�ѯ��ʽΪ���ڶ����ݣ�1���жϲ�ѯ��ʽΪINT����

#define __DEBUG_SN_NEED__           1               //0������Ҫ�������⣻1:��Ҫ��������
#define __DEBUG_BURN__              1               //0����ִ����¼���̣�1����Ҫִ����¼���̣�
#define __DEBUG_DEV_NAME__          1               //0������Ҫ�����豸�㲥����1����Ҫ�������ù㲥��
#define __DEBUG_TX_ID__             0               //0������Ҫ����TX©������1����Ҫ����TX©����
#define __DEBUG_OIP__               1               //0������Ҫ���Թ���������1����Ҫ���Թ�������

#if __DEBUG_DRIVERS__ == 1
#define USE_OS      0
#else
#define USE_OS      1
#endif

#define UART_SCANNER                huart5
#define UART_CH376                  huart4
#define UART_TARGET                 huart3
#define UART_PRINT                  huart2
#define UART_PROTO                  huart2
#define UART_MODBUS                 huart1
#define TIM_COUNT_HANDLE            htim3
#define TIM_MODBUS                  htim2
#define RTC_HANDLE                  hrtc

#if __SPI_USE_SOFTWARE__ == 0
#define SPI_HANDLE                  hspi1
#endif

#define LED_R_TOGGLE()               HAL_GPIO_TogglePin(LED_R_GPIO_Port, LED_R_Pin)
#define LED_G_TOGGLE()               HAL_GPIO_TogglePin(LED_G_GPIO_Port, LED_G_Pin)

#endif //CH376S_YK_CONFIG_H
