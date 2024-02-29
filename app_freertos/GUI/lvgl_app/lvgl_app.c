#include "lvgl_app.h"
#include "lv_port_disp.h"
#include "stdio.h"
#include "app.h"
#include "user_protocol_dbg.h"

lv_obj_t *container = NULL;

TEST_ITEM_s Test_Item_Arr[T_NUM_MAX] = {
        {.name="烧录伏达程序",},
        {.name="白灯电压",},
        {.name="橙灯电压",},
        {.name="蓝灯电压",},
        {.name="软件过压",},
        {.name="软件欠压",},
        {.name="硬件过压",},
        {.name="Q值检测",},
        {.name="Ping频率",},
        {.name="Ping电流",},
        {.name="输出电压",},
        {.name="软件过流",},
        {.name="U盘数据写入",},
        {.name="U盘数据校验",},
};


void lvgl_init(void) {
    lv_init();
    lv_port_disp_init();
}


static void anim_x_cb(void *var, int32_t v) {
    lv_obj_set_x((lv_obj_t *) var, v);
}

static void event_cb_set_page(lv_event_t *e) {
    lv_obj_t *obj = (lv_obj_t *) lv_event_get_target(e);
    uint8_t index = *(uint8_t *) lv_event_get_param(e);

    if (index >= PG_NUM_MAX) {
        return;
    }

    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_set_var(&a, obj);
    lv_anim_set_values(&a, lv_obj_get_x(obj), -240 * index);
    lv_anim_set_time(&a, 0);
    lv_anim_set_exec_cb(&a, anim_x_cb);
    lv_anim_set_path_cb(&a, lv_anim_path_overshoot);
    lv_anim_start(&a);

}

static void event_cb_set_res(lv_event_t *e) {

    lv_obj_t *obj = lv_event_get_target(e);
    TEST_RESULT_s *param = (TEST_RESULT_s *) lv_event_get_param(e);
    if (param) {
        bool need_num = true;
        if (obj == Test_Item_Arr[T_NU1708_BURN].obj
            || obj == Test_Item_Arr[T_UDISK_WRITE].obj
            || obj == Test_Item_Arr[T_UDISK_CHECK].obj
            || obj == Test_Item_Arr[T_SW_OIP].obj
            || obj == Test_Item_Arr[T_SW_OVP].obj
            || obj == Test_Item_Arr[T_SW_UVD].obj
            || obj == Test_Item_Arr[T_HW_OVP].obj) {
            need_num = false;
        }
        char buf[10];
        sprintf(buf, "%lu", param->value);
        if (!param->res) {
            lv_label_set_text_fmt(obj, "#008000 %s OK#", need_num ? buf : "");
        } else {
            lv_label_set_text_fmt(obj, "#FF0000 %s FAIL#", need_num ? buf : "");
        }

    } else {
        lv_label_set_text(obj, "");
    }
}


static void event_cb_test_self(lv_event_t *e) {
    lv_obj_t *obj = (lv_obj_t *) lv_event_get_user_data(e);
    TEST_RESULT_s *param = (TEST_RESULT_s *) lv_event_get_param(e);
    if (param) {
        bool res = param->res;
        if (res) {
            lv_label_set_text_fmt(obj, "#008000 %s 初始化 OK#", param->name);
        } else {
            lv_label_set_text_fmt(obj, "#FF0000 %s 初始化 Err#", param->name);
        }
    } else {
        lv_label_set_text(obj, "");
    }

}

static void event_cb_fill_sn(lv_event_t *e) {
    lv_obj_t *obj = (lv_obj_t *) lv_event_get_user_data(e);
    TEST_RESULT_s *param = (TEST_RESULT_s *) lv_event_get_param(e);
    if (param) {
        if (param->res) {
            lv_label_set_text_fmt(obj, "#008000 %s#", param->name);
        } else {
            lv_label_set_text_fmt(obj, "#FF0000 %s#", param->name);
        }
    } else {
        lv_label_set_text(obj, "");
    }


}

static void event_cb_notice_scan(lv_event_t *e) {
    lv_obj_t *obj = (lv_obj_t *) lv_event_get_user_data(e);
    uint8_t *data = lv_event_get_param(e);
    if (data) {
        lv_label_set_text_fmt(obj, "#0000FF %s#", data);
    } else {
        lv_label_set_text(obj, "");
    }

}

static void event_cb_notice_push_down(lv_event_t *e) {
    lv_obj_t *obj = (lv_obj_t *) lv_event_get_user_data(e);
    uint8_t *data = lv_event_get_param(e);
    if (data) {
        lv_label_set_text_fmt(obj, "#0000FF %s#", data);
    } else {
        lv_label_set_text(obj, "");
    }

}

static void event_cb_report(lv_event_t *e) {
    lv_obj_t *obj = (lv_obj_t *) lv_event_get_user_data(e);
    TEST_RESULT_s *para = (TEST_RESULT_s *) lv_event_get_param(e);
    if (para) {
        if (para->res) {
            lv_label_set_text_fmt(obj, "#008000 %s#", para->name);
        } else {
            lv_label_set_text_fmt(obj, "#FF0000 %s#", para->name);
        }
    } else {
        lv_label_set_text(obj, "");
    }
}

static void event_cb_set_time_token(lv_event_t *e) {
    lv_obj_t *obj = (lv_obj_t *) lv_event_get_user_data(e);
    uint32_t tm_token = *(uint32_t *) lv_event_get_param(e);
    lv_label_set_text_fmt(obj, "#FF0000 %d s#", tm_token);
}

static void timer_cb(lv_timer_t *timer) {
    /*Use the user_data*/
    lv_obj_t *obj = timer->user_data;
    if (Flag_Running) {
        Time_Token_Per++;
        lv_event_send(container, MY_EVENT_SET_TMTK, &Time_Token_Per);
    }

}

static void event_cb_clear(lv_event_t *e) {

    uint8_t idx = PG_SCAN_SN;
    lv_event_send(container, MY_EVENT_SET_PAGE_INDEX, &idx);

    lv_event_send(container, MY_EVENT_SET_TEST_SELF, NULL);

    lv_event_send(container, MY_EVENT_SET_SN, NULL);

    lv_event_send(container, MY_EVENT_SET_SHOW_NOTICE_PUSH_DOWN, NULL);

    uint32_t tm_reset = 0;
    lv_event_send(container, MY_EVENT_SET_TMTK, &tm_reset);

    lv_event_send(container, MY_EVENT_SET_SHOW_NOTICE_SCAN, NULL);

    lv_event_send(container, MY_EVENT_SET_REPORT, NULL);

    for (uint8_t i = 0; i < T_NUM_MAX; i++) {
        lv_event_send(Test_Item_Arr[i].obj, MY_EVENT_SET_RES, NULL);
    }


}


void lvgl_app(void) {

    lv_coord_t scr_width = lv_obj_get_width(lv_scr_act());
    lv_coord_t scr_hight = lv_obj_get_height(lv_scr_act());
#if 0
    lv_obj_t *scr = lv_scr_act();
    lv_obj_t *sp = lv_spinner_create(scr, 1000, 120);
    lv_obj_center(sp);
#else
    //屏幕
    lv_obj_clear_flag(lv_scr_act(), LV_OBJ_FLAG_SCROLLABLE);

    //main container
    container = lv_obj_create(lv_scr_act());
    lv_obj_set_size(container, (lv_coord_t) (scr_width * PG_NUM_MAX), scr_hight);
    lv_obj_set_flex_flow(container, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(container, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_style_pad_all(container, 0, LV_PART_MAIN);
    lv_obj_set_style_border_width(container, 0, LV_PART_MAIN);
    lv_obj_set_style_pad_column(container, 0, LV_PART_MAIN);
    lv_obj_set_style_radius(container, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    //page_scan_sn
    lv_obj_t *page_scan_sn = lv_obj_create(container);
    lv_obj_set_size(page_scan_sn, scr_width, scr_hight);
    lv_obj_set_flex_flow(page_scan_sn, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(page_scan_sn, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_style_pad_all(page_scan_sn, 0, LV_PART_MAIN);
    lv_obj_set_style_border_width(page_scan_sn, 0, LV_PART_MAIN);
    lv_obj_set_style_radius(page_scan_sn, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_t *label_test_self = lv_label_create(page_scan_sn);
    lv_label_set_recolor(label_test_self, true);
    lv_label_set_text(label_test_self, "");

    lv_obj_t *label_pd = lv_label_create(page_scan_sn);
    lv_label_set_recolor(label_pd, true);
    lv_label_set_text(label_pd, "");

    lv_obj_t *label_scan = lv_label_create(page_scan_sn);
    lv_label_set_recolor(label_scan, true);
    lv_label_set_text(label_scan, "");

    lv_obj_t *label_sn = lv_label_create(page_scan_sn);
    lv_label_set_recolor(label_sn, true);
    lv_label_set_text(label_sn, "");

#if 0
    //    //page_pushdown
    //    lv_obj_t * page_pushdown = lv_obj_create(container);
    //    lv_obj_set_size(page_pushdown, scr_width, scr_width);
    //    lv_obj_set_flex_flow(page_pushdown, LV_FLEX_FLOW_COLUMN);
    //    lv_obj_set_flex_align(page_pushdown, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    //    lv_obj_set_style_pad_all(page_pushdown, 0, LV_PART_MAIN);
    //    lv_obj_set_style_border_width(page_pushdown, 0, LV_PART_MAIN);
    //
    //    lv_obj_t * label_pd = lv_label_create(page_pushdown);
    //    lv_label_set_recolor(label_pd, true);
    //    lv_label_set_text(label_pd, "#FF0000 PUSH DOWN!!!#");
#endif

    //page_test
    lv_obj_t *page_test = lv_obj_create(container);
    lv_obj_set_size(page_test, scr_width, scr_hight);

    lv_obj_set_flex_flow(page_test, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(page_test, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START);
    lv_obj_set_style_pad_all(page_test, 0, LV_PART_MAIN);
    lv_obj_set_style_border_width(page_test, 0, LV_PART_MAIN);
    lv_obj_clear_flag(page_test, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_pad_row(page_test, 0, LV_PART_MAIN);
    lv_obj_set_style_radius(page_test, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(page_test, &lv_font_heiti_14, 0);

    for (uint8_t i = 0; i < T_NUM_MAX; i++) {
//        if (ParaGroup.hw_ovp_enable.value == 0) {
//            if (i == T_HW_OVP) {
//                continue;
//            }
//        }
        lv_obj_t *item = lv_obj_create(page_test);
        lv_obj_set_width(item, scr_width);
        lv_obj_set_height(item, LV_SIZE_CONTENT);
        lv_obj_set_flex_flow(item, LV_FLEX_FLOW_ROW);
        lv_obj_set_flex_align(item, LV_FLEX_ALIGN_SPACE_BETWEEN, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_START);
        lv_obj_set_style_pad_all(item, 1, LV_PART_MAIN);
        lv_obj_set_style_border_width(item, 1, LV_PART_MAIN);
        lv_obj_set_style_radius(item, 0, LV_PART_MAIN);

        lv_obj_t *name = lv_label_create(item);
        lv_label_set_text(name, Test_Item_Arr[i].name);
        lv_obj_t *res = lv_label_create(item);
        lv_label_set_recolor(res, true);
        lv_label_set_text(res, "");
        lv_obj_add_event_cb(res, event_cb_set_res, (lv_event_code_t) MY_EVENT_SET_RES, NULL);
        Test_Item_Arr[i].obj = res;
    }

    lv_obj_t *label_report = lv_label_create(page_test);
    lv_label_set_recolor(label_report, true);
    lv_label_set_text(label_report, "");

#if 0
    //page_report
    lv_obj_t *page_report = lv_obj_create(container);
    lv_obj_set_size(page_report, scr_width, scr_hight);

    lv_obj_set_flex_flow(page_report, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(page_report, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START);
    lv_obj_set_style_pad_all(page_report, 0, LV_PART_MAIN);
    lv_obj_set_style_border_width(page_report, 0, LV_PART_MAIN);
    lv_obj_clear_flag(page_report, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_pad_row(page_report, 0, LV_PART_MAIN);

    lv_obj_t *label_pass_or_not = lv_label_create(page_report);
    lv_label_set_recolor(label_pass_or_not, true);
    lv_label_set_text(label_pass_or_not, "");
#endif

    //计时
    lv_obj_t *label_time = lv_label_create(lv_scr_act());
    lv_label_set_recolor(label_time, true);
    lv_label_set_text(label_time, "");
    lv_obj_align(label_time, LV_ALIGN_BOTTOM_RIGHT, 0, 0);
    lv_timer_create(timer_cb, 1000, label_time);


    //容器事件绑定
    lv_obj_add_event_cb(container, event_cb_set_page, (lv_event_code_t) MY_EVENT_SET_PAGE_INDEX, NULL);
    lv_obj_add_event_cb(container, event_cb_test_self, (lv_event_code_t) MY_EVENT_SET_TEST_SELF, label_test_self);
    lv_obj_add_event_cb(container, event_cb_fill_sn, (lv_event_code_t) MY_EVENT_SET_SN, label_sn);
    lv_obj_add_event_cb(container, event_cb_notice_scan, (lv_event_code_t) MY_EVENT_SET_SHOW_NOTICE_SCAN, label_scan);
    lv_obj_add_event_cb(container, event_cb_notice_push_down, (lv_event_code_t) MY_EVENT_SET_SHOW_NOTICE_PUSH_DOWN, label_pd);
    lv_obj_add_event_cb(container, event_cb_report, (lv_event_code_t) MY_EVENT_SET_REPORT, label_report);
    lv_obj_add_event_cb(container, event_cb_set_time_token, (lv_event_code_t) MY_EVENT_SET_TMTK, label_time);
    lv_obj_add_event_cb(container, event_cb_clear, (lv_event_code_t) MY_EVENT_CLEAR, NULL);
#endif
}
