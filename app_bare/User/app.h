//
// Created by Yinke7 on 2023/12/6.
//

#ifndef FRESSRTOS_APP_H
#define FRESSRTOS_APP_H

#include "stm32f1xx_hal.h"
#include "stdbool.h"
#include "spi.h"
#include "usart.h"

#define  UART_RECV_BUFF_LENTH           100
#define UART_MB_RECV_BUFF_LENTH         256

void App_ELog_Init(void);

void App_BSP_Init(void);

void App_Init(void);

void App_Proc(void);

extern uint32_t Time_Token_Per;
extern bool Flag_Running;

extern uint8_t Uart_Recv_Buff[UART_RECV_BUFF_LENTH];
extern uint8_t Uart_MB_Recv_Buff[UART_MB_RECV_BUFF_LENTH];

#endif //FRESSRTOS_APP_H
