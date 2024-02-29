//
// Created by Yinke7 on 2023/12/29.
//

//#include "stm32f4xx_hal.h"
//#include "cmsis_os.h"

#include "mb.h"
#include "mbport.h"
#include "mbutils.h"
#include "app.h"

#define REG_INPUT_START 1
#define REG_INPUT_NREGS 8

static USHORT usRegInputStart = REG_INPUT_START;
static USHORT usRegInputBuf[REG_INPUT_NREGS] = {
        0x11,
        0x22,
        0x33,
        0x44,
        0x55,
        0x66,
        0x77,
        0x88,
};

#define REG_COILS_START 0x100
#define REG_NCOILS 3

typedef union {
    struct {
        uint8_t b0: 1;
        uint8_t b1: 1;
        uint8_t b2: 1;
        uint8_t b3: 1;
        uint8_t b4: 1;
        uint8_t b5: 1;
        uint8_t b6: 1;
        uint8_t b7: 1;
    } c_s;
    uint8_t coil;
} COIL_u;

static COIL_u CoilsBuf[REG_NCOILS] =
        {
                {.coil = 0xff},
                {.coil = 0xbb},
                {.coil = 0xcc},
        };
static UCHAR CoilsBuf_Arr[REG_NCOILS] =
        {
                0xaa,
                0xbb,
                0xcc
        };

void ModbusRTUTask(void const *argument) {

    eMBErrorCode eStatus = eMBInit(MB_RTU, 1, 3, 115200, MB_PAR_NONE);
    eStatus = eMBEnable();

    while (1) {
        eMBPoll();
    }
}

eMBErrorCode
eMBRegInputCB(UCHAR *pucRegBuffer, USHORT usAddress, USHORT usNRegs) {
    eMBErrorCode eStatus = MB_ENOERR;
    int iRegIndex;

    if ((usAddress >= REG_INPUT_START)
        && (usAddress + usNRegs <= REG_INPUT_START + REG_INPUT_NREGS)) {
        iRegIndex = (int) (usAddress - usRegInputStart);
        while (usNRegs > 0) {
            *pucRegBuffer++ =
                    (unsigned char) (usRegInputBuf[iRegIndex] >> 8);
            *pucRegBuffer++ =
                    (unsigned char) (usRegInputBuf[iRegIndex] & 0xFF);
            iRegIndex++;
            usNRegs--;
        }

    } else {

        eStatus = MB_ENOREG;
    }

    return eStatus;
}

eMBErrorCode
eMBRegHoldingCB(UCHAR *pucRegBuffer, USHORT usAddress, USHORT usNRegs,
                eMBRegisterMode eMode) {
    return MB_ENOREG;
}


eMBErrorCode
eMBRegCoilsCB(UCHAR *pucRegBuffer, USHORT usAddress, USHORT usNCoils,
              eMBRegisterMode eMode) {
    eMBErrorCode eStatus = MB_ENOERR;


    if ((usAddress >= REG_COILS_START)  //访问地址大于等于起始地址
        && (usAddress + usNCoils <= REG_COILS_START + REG_NCOILS * 8))  //访问结束地址小于末尾地址
    {
        SHORT iNCoils = (SHORT) usNCoils;
        USHORT usBitOffset = (SHORT) (usAddress - REG_COILS_START);
        switch (eMode) {
            case MB_REG_READ: {     //读线圈
                while (iNCoils > 0) {
                    *pucRegBuffer++ = xMBUtilGetBits(CoilsBuf_Arr, usBitOffset, (UCHAR) (iNCoils > 8 ? 8 : iNCoils));
                    iNCoils -= 8;
                    usBitOffset += 8;
                }
                break;
            }
            case MB_REG_WRITE: {    //写线圈
                while (iNCoils > 0) {
                    xMBUtilSetBits(CoilsBuf_Arr, usBitOffset, (uint8_t) (iNCoils > 8 ? 8 : iNCoils), *pucRegBuffer++);
                    iNCoils -= 8;
                }
                break;
            }


            default:
                break;
        }

    } else {
        eStatus = MB_ENOREG;
    }


    return eStatus;
//    return MB_ENOREG;
}

eMBErrorCode
eMBRegDiscreteCB(UCHAR *pucRegBuffer, USHORT usAddress, USHORT usNDiscrete) {
    return MB_ENOREG;
}