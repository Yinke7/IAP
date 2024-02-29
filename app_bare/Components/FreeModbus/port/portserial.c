/*
 * FreeModbus Libary: BARE Port
 * Copyright (C) 2006 Christian Walter <wolti@sil.at>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * File: $Id$
 */

#include "port.h"

/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mbport.h"

/* ----------------------- static functions ---------------------------------*/
static void prvvUARTTxReadyISR(void);

static void prvvUARTRxISR(void);

static uint8_t Uart_RecvByte_Temp = 0;
//static uint8_t Uart_RecvByte_Temp[] = 0;
/* ----------------------- Start implementation -----------------------------*/
void
vMBPortSerialEnable(BOOL xRxEnable, BOOL xTxEnable) {
    /* If xRXEnable enable serial receive interrupts. If xTxENable enable
     * transmitter empty interrupts.
     */
    if (xRxEnable) {
        HAL_UART_Receive_IT(&UART_MODBUS, (uint8_t *) &Uart_RecvByte_Temp, 1);
    } else {
        HAL_UART_AbortReceive_IT(&UART_MODBUS);
    }
    if (xTxEnable) {
//        uint8_t tmp = 0x33;
//        HAL_UART_Transmit_IT(&UART_MODBUS, (const uint8_t *) &tmp, 1);    //不能使用此 API，因为它必须要求发送一次数据，这会导致发送的数据产生紊乱
        __HAL_UART_ENABLE_IT(&UART_MODBUS, UART_IT_TC);     //只使能 TC 中断，让单片机进入一次中断，以便调用 pxMBFrameCBTransmitterEmpty
    } else {
        HAL_UART_AbortTransmit_IT(&UART_MODBUS);
    }

}

BOOL
xMBPortSerialInit(UCHAR ucPORT, ULONG ulBaudRate, UCHAR ucDataBits, eMBParity eParity) {
//    return FALSE;
    return TRUE;
}

static char ByteSend = 0;
BOOL
xMBPortSerialPutByte(CHAR ucByte) {
    /* Put a byte in the UARTs transmit buffer. This function is called
     * by the protocol stack if pxMBFrameCBTransmitterEmpty( ) has been
     * called. */
//    UART_MODBUS.Instance->DR = ucByte;
    ByteSend = ucByte;
    HAL_UART_Transmit_IT(&UART_MODBUS, (const uint8_t *) &ByteSend, 1);
    return TRUE;
}

BOOL
xMBPortSerialGetByte(CHAR *pucByte) {
    /* Return the byte in the UARTs receive buffer. This function is called
     * by the protocol stack after pxMBFrameCBByteReceived( ) has been called.
     */
//    *pucByte = UART_MODBUS.Instance->DR & 0x1FF;
    *pucByte = Uart_RecvByte_Temp;
    Uart_RecvByte_Temp = 0;
    HAL_UART_Receive_IT(&UART_MODBUS, (uint8_t *) &Uart_RecvByte_Temp, 1);
    return TRUE;
}

/* Create an interrupt handler for the transmit buffer empty interrupt
 * (or an equivalent) for your target processor. This function should then
 * call pxMBFrameCBTransmitterEmpty( ) which tells the protocol stack that
 * a new character can be sent. The protocol stack will then call 
 * xMBPortSerialPutByte( ) to send the character.
 */
static void prvvUARTTxReadyISR( void )
{
    pxMBFrameCBTransmitterEmpty(  );
}

/* Create an interrupt handler for the receive interrupt for your target
 * processor. This function should then call pxMBFrameCBByteReceived( ). The
 * protocol stack will then call xMBPortSerialGetByte( ) to retrieve the
 * character.
 */
static void prvvUARTRxISR( void )
{
    pxMBFrameCBByteReceived(  );
}
