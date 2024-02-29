//
// Created by Yinke7 on 2023/12/6.
//

#include "app_it.h"
#include "usart.h"
#include "stdio.h"
#include "yk_config.h"
#include "protocols/app_UART/user_protocol_Uart.h"


void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size) {
    if (huart->Instance == UART_PRINT.Instance) {
        PT_Transport_Recv(&PT_App_UART, Uart_Recv_Buff, Size);
        HAL_UARTEx_ReceiveToIdle_IT(huart, Uart_Recv_Buff, UART_RECV_BUFF_LENTH);
    }
}