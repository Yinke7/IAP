/*
 * user_protocol_custom.c
 *
 *  Created on: 2022-11-11
 *      Author: Yinke7
 */

#include "user_protocol_Uart.h"
#include "app.h"
#include "app_flash.h"


static unsigned char UART_Recv_Buff[UART_RECV_BUFF_LENTH_MAX] = {0};    //缓冲区

static void UART_Command_Send(unsigned char cmd, unsigned char *buff, unsigned char lenth);

static void UART_Command_Recv(void);

static void UART_Report_Send(unsigned char cmd, unsigned char *buff, unsigned char lenth);

static void UART_Report_Recv(void);

static uint8_t count = 0;

static bool Uart_Flag = false;        //标记是否处于测试模式

DBG_PARA_KEY_VALUE_s KVP_Write[DBG_COUNT_MAX] = {0};

DBG_PARA_KEY_VALUE_s KVP_Read[DBG_COUNT_MAX] = {0};

DBG_PARA_GROUP_s ParaGroup = {
        .if_5v_l = {
                .value = 123456,
                .cmd = DBG_IF_5V_L,
                .desc = "IF5V_L",
                .prop_s.p_s.write = 1,
                .prop_s.p_s.read = 1,
                .prop_s.p_s.flash = 1,
        },
        .if_5v_h = {
                .value = 234567,
                .cmd = DBG_IF_5V_H,
                .desc = "IF5V_H",
                .prop_s.p_s.write = 1,
                .prop_s.p_s.read = 1,
                .prop_s.p_s.flash = 1,
        },
};

PT_APPLICATION_s PT_App_UART =
        {
                .lock = false,
                .pbuff = UART_Recv_Buff,
                .buff_size = UART_RECV_BUFF_LENTH_MAX,
                .index_write = 0,
                .index_read = 0,
                .lenth_written = 0,
                .frame_send = {0},
                .frame_recv = {0},
                .pfunc_command_recv = UART_Command_Recv,
                .pfunc_report_send = UART_Report_Send,
        };


static unsigned int DBG_U8Array_To_U32(unsigned char *array) {
    unsigned int res = array[3] << 24
                       | array[2] << 16
                       | array[1] << 8
                       | array[0];
    return res;
}

static void DBG_U32_To_U8Array(unsigned int value, unsigned char *array) {
    array[0] = value & 0xff;
    array[1] = (value >> 8) & 0xff;
    array[2] = (value >> 16) & 0xff;
    array[3] = (value >> 24) & 0xff;
}


void DBG_Read_Params_FromFlash(void) {
    unsigned int lenth = DBG_COUNT_MAX * sizeof(DBG_PARA_KEY_VALUE_s);
    Flash_Read(FLASH_ADDR_PARA_SETTTING, (unsigned short *) KVP_Read, lenth / 2);
}

void DBG_Write_Params_ToFlash(void) {
    //筛选
    unsigned int offset = 0;
    for (unsigned char idx = 0; idx < sizeof(DBG_PARA_GROUP_s) / sizeof(DBG_PARA_ITEM_s); idx++) {
        DBG_PARA_ITEM_s *item = (DBG_PARA_ITEM_s *) &ParaGroup + idx;
        if (item->prop_s.p_s.flash == 1) {
            KVP_Write[offset].cmd = item->cmd;
            KVP_Write[offset].value = item->value;
            offset++;
        }
    }
#if 0
    //操作Flash
    unsigned int lenth = offset * sizeof(DBG_PARA_KEY_VALUE_s);
    flash_erase_sector(FLASH_ADDR_PARA_SETTTING);        //先擦除扇区
    if (lenth > 4096)        //扇区最大长度为4K，超扇区范围
    {
        return;
    }
    u32 mul = lenth / 256;    //每个page最大写入长度256byte，求整
    u32 left = lenth % 256;    //每个page最大写入长度256byte，求余
    u32 i = 0;
    for (i = 0; i < mul; i++) {
        flash_write_page(FLASH_ADDR_PARA_SETTTING + (i * 256), 256, (unsigned char *) KVP_Write + (i * 256));
    }

    if (left != 0) {
        flash_write_page(FLASH_ADDR_PARA_SETTTING + (i * 256), left, (unsigned char *) KVP_Write + (i * 256));
    }
#else
    unsigned int lenth = offset * sizeof(DBG_PARA_KEY_VALUE_s);
    Flash_Write(FLASH_ADDR_PARA_SETTTING, (unsigned short *) KVP_Write, lenth / 2);

#endif
}


unsigned int DBG_Init_Params(void) {
    memset(KVP_Read, 0xff, DBG_COUNT_MAX * sizeof(DBG_PARA_KEY_VALUE_s));    //先将读缓存全置0xff

    DBG_Read_Params_FromFlash();

    unsigned int cnt = 0;
    for (cnt = 0; cnt < DBG_COUNT_MAX; cnt++) {
        if (KVP_Read[cnt].cmd == 0xff && KVP_Read[cnt].value == 0xffffffff) break;

        for (unsigned char idx = 0; idx < sizeof(DBG_PARA_GROUP_s) / sizeof(DBG_PARA_ITEM_s); idx++) {
            DBG_PARA_ITEM_s *item = (DBG_PARA_ITEM_s *) &ParaGroup + idx;
            if (KVP_Read[cnt].cmd == item->cmd) {
                item->value = KVP_Read[cnt].value;
            }
        }
    }

    return cnt;

}


/**
 * 发送命令帧
 */
static void UART_Command_Send(unsigned char cmd, unsigned char *buff, unsigned char lenth) {
    //TODO 用户自己实现
}


/**
 * 接收命令帧
 */
static void UART_Command_Recv(void) {
    unsigned char chksum[2] = {0};
    PT_CheckSum(&PT_App_UART.frame_recv, chksum);
    if (memcmp(PT_App_UART.frame_recv.Checksum, chksum, sizeof(PT_App_UART.frame_recv.Checksum)) == 0) {
        switch (PT_App_UART.frame_recv.Command) {
//            case RC_CMD_RTC_SET: {
//
//                PT_PACK_UART_u *pack = (PT_PACK_UART_u *) PT_App_UART.frame_recv.Data;
//                App_RTC_Set(pack->rtc_set_s.data);
//
//                UART_Report_Send(RC_CMD_RTC_SET, (unsigned char *) pack, 0);
//                break;
//            }

            case CB_DBG_REQUEST: {

                Uart_Flag = true;        //置位标志
                UART_Report_Send(PT_App_UART.frame_recv.Command, (unsigned char *) &Uart_Flag, 1);
                break;
            }

            case CB_DBG_EXIT: {
#if __DEBUG_FLASH_DATA_SYNC__ == 1
                DBG_Write_Params_ToFlash();
#endif
                Uart_Flag = false;
                break;
            }

            case CB_GET_VER: {

                char ver[128];
                sprintf(ver, "%s, %s%s, PT_%s, PT_DBG_%s\n",
                        HW_VERSION,
                        PRODUCT_NO,
                        SW_VERSION,
                        PT_VERSION,
                        PT_DBG_VERSION);
                unsigned int lenth = strlen(ver);
                unsigned int pack_leth = PT_DATA_LENTH_MAX - 1;
                unsigned char multiple = lenth / pack_leth;            //求整，计算有多少个完整的包
                unsigned char left = lenth % pack_leth;                //求余，计算最后一个包的字节长度，可能为0
                unsigned char idx = 0;                                        //包序号
                PT_PACK_UART_u pack = {0};
                if (lenth == 0)    //长度为0
                {
                    pack.ver_s.index = 0x80 | ((++idx) & 0x7f);    // index =>[b7]结束包标志位,[b6:b0]包序号
                    sprintf(pack.ver_s.data, "%s", "NO VER");
                    UART_Report_Send(PT_App_UART.frame_recv.Command, (unsigned char *) &pack, PT_DATA_LENTH_MAX);
                }
                if (left != 0)    //有不完整包
                {
                    for (unsigned char i = 0; i < multiple; i++) {
                        pack.ver_s.index = 0x00 | ((++idx) & 0x7f);    // index =>[b7]结束包标志位,[b6:b0]包序号
                        memcpy(pack.ver_s.data, ver + i * pack_leth, pack_leth);
                        UART_Report_Send(PT_App_UART.frame_recv.Command, (unsigned char *) &pack, PT_DATA_LENTH_MAX);
                    }
                    pack.ver_s.index = 0x80 | ((++idx) & 0x7f);        // index =>[b7]结束包标志位,[b6:b0]包序号
                    memcpy(pack.ver_s.data, ver + multiple * pack_leth, pack_leth);
                    UART_Report_Send(PT_App_UART.frame_recv.Command, (unsigned char *) &pack, left);
                } else            //全是完整包
                {
                    for (unsigned char i = 0; i < multiple; i++) {
                        pack.ver_s.index = (i == (multiple - 1) ? 0x80 : 0x00) | ((++idx) & 0x7f);        // index =>[b7]结束包标志位,[b6:b0]包序号
                        memcpy(pack.ver_s.data, ver + i * pack_leth, pack_leth);
                        UART_Report_Send(PT_App_UART.frame_recv.Command, (unsigned char *) &pack, PT_DATA_LENTH_MAX);
                    }
                }
                break;
            }

            case CB_GET_PARA_COUNT: {
                unsigned int cnt = 0;
                for (unsigned char idx = 0; idx < sizeof(DBG_PARA_GROUP_s) / sizeof(DBG_PARA_ITEM_s); idx++) {
                    DBG_PARA_ITEM_s *item = (DBG_PARA_ITEM_s *) &ParaGroup + idx;
                    if (item->prop_s.p_s.read == 1) {
                        cnt++;
                    }
                }
                unsigned char buf[4];
                DBG_U32_To_U8Array(cnt, buf);
                UART_Report_Send(PT_App_UART.frame_recv.Command, buf, sizeof(buf));
                break;
            }

            case CB_READALL_PARAS: {
                for (unsigned char idx = 0; idx < sizeof(DBG_PARA_GROUP_s) / sizeof(DBG_PARA_ITEM_s); idx++) {
                    DBG_PARA_ITEM_s *item = (DBG_PARA_ITEM_s *) &ParaGroup + idx;
                    if (item->prop_s.p_s.read == 1) {
                        UART_Report_Send(PT_App_UART.frame_recv.Command, &item->cmd, 1);
                    }
                }

                break;
            }

            case CB_PARA_WRITE: {
                PT_PACK_UART_u *pack = (PT_PACK_UART_u *) PT_App_UART.frame_recv.Data;
                for (unsigned char idx = 0; idx < sizeof(DBG_PARA_GROUP_s) / sizeof(DBG_PARA_ITEM_s); idx++) {
                    DBG_PARA_ITEM_s *item = (DBG_PARA_ITEM_s *) &ParaGroup + idx;
                    if (item->cmd == pack->para_s.cmd) {
                        if (item->value != DBG_U8Array_To_U32(pack->para_s.value)) {
                            item->value = DBG_U8Array_To_U32(pack->para_s.value);
                        }
                        UART_Report_Send(PT_App_UART.frame_recv.Command, &item->cmd, 1);
                    }
                }
                break;
            }

            case CB_PARA_READ: {
                PT_PACK_UART_u *pack = (PT_PACK_UART_u *) PT_App_UART.frame_recv.Data;
                UART_Report_Send(PT_App_UART.frame_recv.Command, &pack->para_s.cmd, 1);
                break;
            }

            default:
                break;
        }
    }
}


/**
 * 发送上报帧
 */
static void UART_Report_Send(unsigned char cmd, unsigned char *buff, unsigned char lenth) {

    PT_FRAME_s frame = {0};
    PT_PACK_UART_u pack = {0};


    switch (cmd) {
        case RC_CMD_RTC_SET: {

            break;
        }

        case CB_DBG_REQUEST: {
            pack.request_s.flag = buff[0];
            memset(pack.request_s.res, 0, sizeof(pack.request_s.res));
            break;
        }

        case CB_GET_VER: {
            memcpy(&pack, buff, lenth);
            break;
        }

        case CB_GET_PARA_COUNT: {
            memcpy(pack.get_para_count_s.count, buff, sizeof(pack.get_para_count_s.count));
            memset(pack.get_para_count_s.res, 0, sizeof(pack.get_para_count_s.res));
            break;
        }

        case CB_READALL_PARAS:
        case CB_PARA_READ: {
            for (unsigned char idx = 0; idx < sizeof(DBG_PARA_GROUP_s) / sizeof(DBG_PARA_ITEM_s); idx++) {
                DBG_PARA_ITEM_s *item = (DBG_PARA_ITEM_s *) &ParaGroup + idx;
                if (item->cmd == buff[0]) {
                    pack.para_s.cmd = buff[0];
                    pack.para_s.prop_s.properties = item->prop_s.properties;
                    DBG_U32_To_U8Array(item->value, pack.para_s.value);
                    memcpy(pack.para_s.desc, item->desc, sizeof(pack.para_s.desc));
                }
            }
        }


        case CB_PARA_WRITE: {
            for (unsigned char idx = 0; idx < sizeof(DBG_PARA_GROUP_s) / sizeof(DBG_PARA_ITEM_s); idx++) {
                DBG_PARA_ITEM_s *item = (DBG_PARA_ITEM_s *) &ParaGroup + idx;
                if (item->cmd == buff[0]) {
                    pack.para_s.cmd = buff[0];
                }
            }
        }

        case RC_CMD_ERR: {

            break;
        }

        default:
            break;

    }


    frame.Header = PT_FRAME_HEADER;
    frame.Lenth = PT_FRAME_LENTH;
    frame.Count = count;
    frame.Command = cmd;
    memcpy(frame.Data, &pack, PT_DATA_LENTH_MAX);
    PT_CheckSum(&frame, frame.Checksum);
    count++;

    //发送
    HAL_UART_Transmit(&UART_PRINT, (unsigned char *) &frame, sizeof(frame), 0xff);

}


/**
 * 接收上报帧
 */
static void UART_Report_Recv(void) {
    //TODO 用户自己实现
}


