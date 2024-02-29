//
// Created by Yinke7 on 2023/12/6.
//

#include "app_it.h"
#include "stdio.h"
#include "user_protocol_dbg.h"
#include "app_task.h"
#include "user_config.h"
#include "mb.h"
#include "mbport.h"
#include "port.h"

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size) {

    if (huart->Instance == UART_PROTO.Instance) {
        PT_Transport_Recv(&PT_App_DBG, Uart_Recv_Buff, Size);
        if (Binarysemphr_Uart_Protocol) {
            BaseType_t xHigherPriorityTaskWoken = pdFALSE;
            xSemaphoreGiveFromISR(Binarysemphr_Uart_Protocol, &xHigherPriorityTaskWoken);
            portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
        }
        HAL_UARTEx_ReceiveToIdle_IT(huart, Uart_Recv_Buff, UART_RECV_BUFF_LENTH);
    }
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
    if (huart->Instance == UART_MODBUS.Instance) {
        pxMBFrameCBByteReceived();
    }
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart) {
    if (huart->Instance == UART_MODBUS.Instance) {
        pxMBFrameCBTransmitterEmpty();
    }
}

void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi) {
    if (hspi->Instance == SPI_HANDLE.Instance) {
        if (Binarysemphr_HardWare_Spi) {
            BaseType_t xHigherPriorityTaskWoken = pdFALSE;
            xSemaphoreGiveFromISR(Binarysemphr_HardWare_Spi, &xHigherPriorityTaskWoken);
            portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
        }
    }
}