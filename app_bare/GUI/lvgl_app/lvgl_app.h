#ifndef __LVGL_APP_H
#define __LVGL_APP_H

#include "stm32f1xx_hal.h"
#include "lvgl.h"


enum {
    MY_EVENT_SET_PAGE_INDEX = _LV_EVENT_LAST + 1,
    MY_EVENT_SET_RES,
    MY_EVENT_SET_TEST_SELF,
    MY_EVENT_SET_SN,
    MY_EVENT_SET_SHOW_NOTICE_SCAN,
    MY_EVENT_SET_SHOW_NOTICE_PUSH_DOWN,
    MY_EVENT_SET_REPORT,
    MY_EVENT_SET_TMTK,
    MY_EVENT_CLEAR,

};

enum {
    T_NU1708_BURN = 0,
    T_LED_W,
    T_LED_O,
    T_LED_B,
    T_SW_OVP,
    T_SW_UVD,
    T_HW_OVP,
    T_QVALUE,
    T_PING_FREQ,
    T_TAR_IF_5V_P,
    T_BRD_2V5_T,
    T_SW_OIP,
    T_UDISK_WRITE,
    T_UDISK_CHECK,
    T_NUM_MAX,
};
enum {
    PG_SCAN_SN = 0,
//    PG_PUSH_DOWN,
    PG_TEST,
//    PG_REPORT,
    PG_NUM_MAX,
};

typedef struct {
    char *name;
    uint32_t value;
    bool res;
} TEST_RESULT_s;

typedef struct {
    lv_obj_t *obj;
    const char *name;
} TEST_ITEM_s;


void lvgl_init(void);

void lvgl_app(void);


extern lv_obj_t *cb_sn;
extern lv_obj_t *container;
extern TEST_ITEM_s Test_Item_Arr[T_NUM_MAX];

#endif
