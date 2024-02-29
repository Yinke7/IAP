//
// Created by Yinke7 on 2023/12/14.
//

#ifndef FRESSRTOS_APP_TASK_H
#define FRESSRTOS_APP_TASK_H

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

void osTaskInit(void);

extern xSemaphoreHandle Mutex_Log;
extern xSemaphoreHandle Binarysemphr_Uart_Protocol;
extern xSemaphoreHandle Binarysemphr_HardWare_Spi;

#endif //FRESSRTOS_APP_TASK_H
