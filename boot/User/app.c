//
// Created by Yinke7 on 2023/12/6.
//

#include "app.h"
#include "app_flash.h"
#include "menu.h"
#include "stm32f1xx_hal.h"

uint8_t Uart_Recv_Buff[UART_RECV_BUFF_LENTH];

uint8_t Step_Run = 0;

typedef void (*pjumfunc)(void);

pjumfunc jumpapp;


void App_Jump(uint32_t app_addr) {

    //一定要关闭用到的中断，防进入 App 后发生异常
    SysTick->CTRL = 0;
    SysTick->VAL = 0;
    SysTick->LOAD = 0;

    volatile uint32_t jump_addr = *(volatile uint32_t *) (FLASH_APP_CODE_ADDR + 4);
    jumpapp = (pjumfunc) jump_addr;
    __set_MSP(*(volatile uint32_t *) FLASH_APP_CODE_ADDR);
    jumpapp();

}

void App_Process(void) {
    if (Step_Run == 0) {
//        if (Update_Check_Normal()) {
//            printf("not need update, going to app...\n");
//            /*
//             * 跳转
//             * */
//            HAL_Delay(100);
//            App_Jump(FLASH_APP_CODE_ADDR);
//        } else {
//            printf("need update, waitting data...\n");
//            Step_Run++;
//        }

//        printf("jump\n");
//        HAL_Delay(100);
//        App_Jump(FLASH_APP_CODE_ADDR);
//        Step_Run++;

        Main_Menu();

    } else {

    }
}


