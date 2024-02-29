//
// Created by Yinke7 on 2023/12/14.
//

#include "app.h"
#include "user_config.h"
#include "app_task.h"
#include "user_protocol.h"
#include "user_protocol_dbg.h"
#include "lvgl_app/lvgl_app.h"
#include "elog.h"
#include "mb.h"
#include "mbport.h"

#define TAG     "app_task"

xSemaphoreHandle Mutex_Log = NULL;
xSemaphoreHandle Binarysemphr_Uart_Protocol = NULL;
xSemaphoreHandle Binarysemphr_HardWare_Spi = NULL;
xSemaphoreHandle Binarysemphr_Lvgl = NULL;

xTaskHandle task_handle_log;

static void task_log(void *args);

xTaskHandle task_handle_uart_protocol;

static void task_uart_protocol(void *args);

xTaskHandle task_handle_gui;

static void task_gui(void *args);

xTaskHandle task_handle_app;

static void task_app(void *args);

xTaskHandle task_handle_mb;

static void task_mb(void *args);

void osTaskInit(void) {
    //创建信号量
    Mutex_Log = xSemaphoreCreateMutex();
    Binarysemphr_Uart_Protocol = xSemaphoreCreateBinary();
    Binarysemphr_HardWare_Spi = xSemaphoreCreateBinary();
    Binarysemphr_Lvgl = xSemaphoreCreateBinary();

    //创建任务
    BaseType_t ret = pdFALSE;
//    ret = xTaskCreate(task_log, "task_log", 128 * 2, NULL, 0, &task_handle_log);
//    configASSERT(ret);
    ret = xTaskCreate(task_uart_protocol, "protocol", 128 * 2, NULL, 3, &task_handle_uart_protocol);
    configASSERT(ret);
    ret = xTaskCreate(task_gui, "gui", 128 * 4, NULL, 0, &task_handle_gui);
    configASSERT(ret);
    ret = xTaskCreate(task_app, "app", 128 * 4, NULL, 2, &task_handle_app);
    configASSERT(ret);
    ret = xTaskCreate(task_mb, "mb", 128 * 6, NULL, 0, &task_handle_mb);
    configASSERT(ret);

    //释放信号量
    xSemaphoreGive(Mutex_Log);
    xSemaphoreGive(Binarysemphr_HardWare_Spi);

    //启动调度
    vTaskStartScheduler();
}

static void task_log(void *args) {
    for (;;) {
//        elog_i(TAG, "hello world");
        vTaskDelay(1000);
    }
}


static void task_uart_protocol(void *args) {
    DBG_Init_Params();
    DBG_Version_Combine(HW_VERSION, SW_VERSION, "NOBLE");

    HAL_UARTEx_ReceiveToIdle_IT(&UART_PROTO, Uart_Recv_Buff, UART_RECV_BUFF_LENTH);

    for (;;) {
        if (pdTRUE == xSemaphoreTake(Binarysemphr_Uart_Protocol, portMAX_DELAY)) {
            if (PT_Transport_Alaysis(&PT_App_DBG)) {
                if (pdTRUE == xSemaphoreTake(Mutex_Log, portMAX_DELAY)) {
                    PT_App_DBG.pfunc_command_recv();
                    xSemaphoreGive(Mutex_Log);
                }
            }
        }
    }
}

static void task_gui(void *args) {
    //初始化
    lvgl_init();

    //应用程序
    lvgl_app();

    //释放信号量
    xSemaphoreGive(Binarysemphr_Lvgl);

    //运行
    for (;;) {
        lv_task_handler();
        vTaskDelay(1);
    }

}

static void task_app(void *args) {

    App_Init();

    elog_w(TAG, "wait lvgl start");
    if (pdTRUE == xSemaphoreTake(Binarysemphr_Lvgl, portMAX_DELAY)) {
        elog_i(TAG, "app start");
    }

    TickType_t ticklast = xTaskGetTickCount();
    for (;;) {
        App_Proc();
        vTaskDelayUntil(&ticklast, 500);
        ticklast = xTaskGetTickCount();
    }
}

#define REG_INPUT_START 1000
#define REG_INPUT_NREGS 8

void task_mb(void *args) {

    eMBErrorCode eStatus = eMBInit(MB_RTU, 1, 3, 115200, MB_PAR_NONE);
    eStatus = eMBEnable();

    while (1) {
        eMBPoll();
        vTaskDelay(1);
    }
}


void vApplicationStackOverflowHook(TaskHandle_t xTask,
                                   char *pcTaskName) {
    elog_a(TAG, "%s StackOver", pcTaskName);
    for (;;) {

    }
}

void vApplicationTickHook(void) {
    if (taskSCHEDULER_RUNNING == xTaskGetSchedulerState()) {
        lv_tick_inc(1);
    }
}

void vApplicationIdleHook(void) {
//    if (pdTRUE == xSemaphoreTake(Mutex_Log, portMAX_DELAY)) {
//        printf("ilde\n");
//        xSemaphoreGive(Mutex_Log);
//    }
}
