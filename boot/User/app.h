//
// Created by Yinke7 on 2023/12/6.
//

#ifndef FRESSRTOS_APP_H
#define FRESSRTOS_APP_H

#include "stm32f1xx_hal.h"


#define  UART_RECV_BUFF_LENTH           100


void App_Jump(uint32_t app_addr);

void App_Process(void);


extern uint8_t Uart_Recv_Buff[UART_RECV_BUFF_LENTH];

#endif //FRESSRTOS_APP_H
