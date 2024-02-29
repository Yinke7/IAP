//
// Created by Yinke7 on 2023/12/6.
//

#include "app.h"
//#include "elog.h"
#include "stdio.h"
#include "LCD_Test.h"
#include "lvgl_app.h"


#define TAG    "app"

static uint8_t Step_Run = 0;
uint32_t Time_Token_Per = 0;
bool Flag_Running = false;

uint8_t Uart_Recv_Buff[UART_RECV_BUFF_LENTH];
uint8_t Uart_MB_Recv_Buff[UART_MB_RECV_BUFF_LENTH];


//void App_ELog_Init(void) {
//    //log
//    setbuf(stdout, NULL);
//    elog_init();
//    elog_set_fmt(ELOG_LVL_ASSERT, ELOG_FMT_TAG | ELOG_FMT_TIME | ELOG_FMT_DIR | ELOG_FMT_FUNC | ELOG_FMT_LINE | ELOG_FMT_LVL);
//    elog_set_fmt(ELOG_LVL_WARN, ELOG_FMT_TAG | ELOG_FMT_TIME | ELOG_FMT_DIR | ELOG_FMT_FUNC | ELOG_FMT_LINE | ELOG_FMT_LVL);
//    elog_set_fmt(ELOG_LVL_INFO, ELOG_FMT_TAG | ELOG_FMT_TIME | ELOG_FMT_LVL);
//    elog_set_fmt(ELOG_LVL_DEBUG, ELOG_FMT_TAG | ELOG_FMT_TIME | ELOG_FMT_LVL);
//    elog_set_fmt(ELOG_LVL_VERBOSE, ELOG_FMT_TAG | ELOG_FMT_TIME | ELOG_FMT_LVL);
//    elog_start();
//}

//void App_BSP_Init(void) {
//
//    //LCD
//    elog_i(TAG, "Lcd Init");
//    LCD_2in4_test_init();
//
//}

void App_Init(void) {
    //Todo
}

void App_Proc(void) {
    if (Step_Run == 0) {
        Flag_Running = true;
        Time_Token_Per = 0;
        lv_event_send(container, MY_EVENT_CLEAR, NULL);
        Step_Run++;
    } else if (Step_Run <= 1) {
        Step_Run++;
    } else if (Step_Run == 2) {
        TEST_RESULT_s res = {
                .res = true,
                .name = "LCD"
        };
        lv_event_send(container, MY_EVENT_SET_TEST_SELF, &res);
        Step_Run++;
    } else if (Step_Run <= 3) {
        lv_event_send(container, MY_EVENT_SET_SHOW_NOTICE_PUSH_DOWN, "下压！");
        Step_Run++;
    } else if (Step_Run <= 4) {
        lv_event_send(container, MY_EVENT_SET_SHOW_NOTICE_SCAN, "扫码！");
        Step_Run++;
    } else if (Step_Run <= 5) {
        TEST_RESULT_s res = {
                .res = true,
                .name = "WWTX123456"
        };
        lv_event_send(container, MY_EVENT_SET_SN, &res);
        Step_Run++;
    } else if (Step_Run <= 6) {
        uint8_t idx = PG_TEST;
        lv_event_send(container, MY_EVENT_SET_PAGE_INDEX, &idx);
        Step_Run++;
    } else if (Step_Run <= 7) {
        TEST_RESULT_s res;
        res.res = false;
        res.value = 12345678;
        lv_event_send(Test_Item_Arr[T_NU1708_BURN].obj, MY_EVENT_SET_RES, &res);
        lv_event_send(Test_Item_Arr[T_LED_W].obj, MY_EVENT_SET_RES, &res);
        lv_event_send(Test_Item_Arr[T_LED_O].obj, MY_EVENT_SET_RES, &res);
        lv_event_send(Test_Item_Arr[T_LED_B].obj, MY_EVENT_SET_RES, &res);
        lv_event_send(Test_Item_Arr[T_SW_OVP].obj, MY_EVENT_SET_RES, &res);
        lv_event_send(Test_Item_Arr[T_SW_UVD].obj, MY_EVENT_SET_RES, &res);
        lv_event_send(Test_Item_Arr[T_HW_OVP].obj, MY_EVENT_SET_RES, &res);
        lv_event_send(Test_Item_Arr[T_QVALUE].obj, MY_EVENT_SET_RES, &res);
        lv_event_send(Test_Item_Arr[T_PING_FREQ].obj, MY_EVENT_SET_RES, &res);
        lv_event_send(Test_Item_Arr[T_TAR_IF_5V_P].obj, MY_EVENT_SET_RES, &res);
        lv_event_send(Test_Item_Arr[T_BRD_2V5_T].obj, MY_EVENT_SET_RES, &res);
        lv_event_send(Test_Item_Arr[T_SW_OIP].obj, MY_EVENT_SET_RES, &res);
        lv_event_send(Test_Item_Arr[T_UDISK_WRITE].obj, MY_EVENT_SET_RES, &res);
        lv_event_send(Test_Item_Arr[T_UDISK_CHECK].obj, MY_EVENT_SET_RES, &res);

        Step_Run++;
    } else if (Step_Run <= 8) {
        Step_Run++;
    } else if (Step_Run <= 9) {
        Step_Run = 0;
    }


    elog_i(TAG, "S:%d", Step_Run);
}


