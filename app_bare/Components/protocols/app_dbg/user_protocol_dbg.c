/*
 * user_protocol_dbg.c
 *
 *  Created on: 2023-2-8
 *      Author: Yinke7
 */
#include "user_protocol_dbg.h"
/* user code begain*/
#include "string.h"
#include "stdio.h"
#include "user_config.h"
#include "app_flash.h"

#define u_sprintf sprintf

typedef unsigned int u32;
typedef unsigned short u16;
typedef unsigned char u8;
/*user code end*/

static unsigned char count = 0;
static unsigned char Recv_Buff_DBG[RECV_BUFF_DBG_LENTH_MAX] = {0};    //缓冲区
static unsigned char Version_buff[DBG_VERSION_BUFF_LENTH] = {0};    //软件版本

static unsigned int DBG_U8Array_To_U32(unsigned char *array);

static void DBG_U32_To_U8Array(unsigned int value, unsigned char *array);

static void DBG_Uart_Send_Data(unsigned char *buff, unsigned char lenth, unsigned int block_ms);

static void DBG_Command_Send(unsigned char cmd, unsigned char *buff, unsigned char lenth);

static void DBG_Command_Recv(void);

static void DBG_Report_Send(unsigned char cmd, unsigned char *buff, unsigned char lenth);

static void DBG_Report_Recv(void);

DBG_PARA_KEY_VALUE_s KVP_Write[DBG_COUNT_MAX] = {0};
DBG_PARA_KEY_VALUE_s KVP_Read[DBG_COUNT_MAX] = {0};


unsigned char DBG_Para_Mark[DBG_COUNT_MAX] = {0};

bool DBG_Flag = false;        //标记是否处于测试模式

DBG_PARA_GROUP_s ParaGroup = {
#define X_MACRO(n, v, c, d, w, r, f) \
    .n = {                                         \
        .value = v,                                \
        .cmd = c,                                  \
        .desc = d,                                  \
        .prop_s.p_s.write = w,                     \
        .prop_s.p_s.read = r,                      \
        .prop_s.p_s.flash = f,                     \
    },
        DBG_TAB
#undef X_MACRO
};


PT_APPLICATION_s PT_App_DBG =
        {
                .lock = false,
                .pbuff = Recv_Buff_DBG,
                .buff_size = RECV_BUFF_DBG_LENTH_MAX,
                .index_write = 0,
                .index_read = 0,
                .lenth_written = 0,
                .frame_send = {0},
                .frame_recv = {0},
                .pfunc_command_recv = DBG_Command_Recv,
                .pfunc_report_send = DBG_Report_Send,
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

static void DBG_Uart_Send_Data(unsigned char *buff, unsigned char lenth, unsigned int block_ms) {
//    extern uart_data_t trans_buff;
//    unsigned long ref = clock_time();
//
//    do {
//        if (reg_uart_status1 & FLD_UART_TX_DONE) {
//            trans_buff.dma_len = lenth;
//            memcpy(trans_buff.data, buff, lenth);
//            uart_dma_send((unsigned short *) &trans_buff);
//            break;
//        }
//    } while (!clock_time_exceed(ref, block_ms * 1000));
    HAL_UART_Transmit(&UART_PROTO, buff, lenth, 0xff);
}


static void DBG_Command_Send(unsigned char cmd, unsigned char *buff, unsigned char lenth) {

}

static void DBG_Command_Recv(void) {
    unsigned char chksum[2] = {0};
    PT_CheckSum(&PT_App_DBG.frame_recv, chksum);
    if (memcmp(PT_App_DBG.frame_recv.Checksum, chksum, sizeof(PT_App_DBG.frame_recv.Checksum)) == 0) {
        switch (PT_App_DBG.frame_recv.Command) {
            case CB_DBG_REQUEST: {

                DBG_Flag = true;        //置位标志
                DBG_Report_Send(PT_App_DBG.frame_recv.Command, &DBG_Flag, 1);
                break;
            }

            case CB_DBG_EXIT: {
                DBG_Write_Params_ToFlash();
                DBG_Flag = false;
                DBG_Report_Send(PT_App_DBG.frame_recv.Command, NULL, 0);
                break;
            }

            case CB_GET_VER: {
                char *ver = Version_buff;
                unsigned int lenth = strlen(ver);
                unsigned int pack_leth = PT_DATA_LENTH_MAX - 1;
                unsigned char multiple = lenth / pack_leth;            //求整，计算有多少个完整的包
                unsigned char left = lenth % pack_leth;                //求余，计算最后一个包的字节长度，可能为0
                unsigned char idx = 0;                                        //包序号
                PT_PACK_DBG_u pack = {0};

                if (lenth == 0)    //长度为0
                {
                    pack.ver_s.index = 0x80 | ((++idx) & 0x7f);    // index =>[b7]结束包标志位,[b6:b0]包序号
                    u_sprintf(pack.ver_s.data, "%s", "NO VER");
                    DBG_Report_Send(PT_App_DBG.frame_recv.Command, &pack, PT_DATA_LENTH_MAX);
                }

                if (left != 0)    //有不完整包
                {
                    for (u8 i = 0; i < multiple; i++) {
                        pack.ver_s.index = 0x00 | ((++idx) & 0x7f);    // index =>[b7]结束包标志位,[b6:b0]包序号
                        memcpy(pack.ver_s.data, ver + i * pack_leth, pack_leth);
                        DBG_Report_Send(PT_App_DBG.frame_recv.Command, &pack, PT_DATA_LENTH_MAX);
                    }
                    pack.ver_s.index = 0x80 | ((++idx) & 0x7f);        // index =>[b7]结束包标志位,[b6:b0]包序号
                    memcpy(pack.ver_s.data, ver + multiple * pack_leth, pack_leth);
                    DBG_Report_Send(PT_App_DBG.frame_recv.Command, &pack, PT_DATA_LENTH_MAX);
                } else            //全是完整包
                {
                    for (u8 i = 0; i < multiple; i++) {
                        pack.ver_s.index = (i == (multiple - 1) ? 0x80 : 0x00) | ((++idx) & 0x7f);        // index =>[b7]结束包标志位,[b6:b0]包序号
                        memcpy(pack.ver_s.data, ver + i * pack_leth, pack_leth);
                        DBG_Report_Send(PT_App_DBG.frame_recv.Command, &pack, PT_DATA_LENTH_MAX);
                    }
                }
                break;
            }

            case CB_GET_PARA_COUNT: {
                unsigned int cnt = 0;
                for (unsigned char idx = 0; idx < DBG_COUNT_MAX; idx++) {
                    DBG_PARA_ITEM_s *item = (DBG_PARA_ITEM_s *) &ParaGroup + idx;
                    if (item->prop_s.p_s.read == 1) {
                        cnt++;
                    }
                }
                unsigned char buf[4];
                DBG_U32_To_U8Array(cnt, buf);
                DBG_Report_Send(PT_App_DBG.frame_recv.Command, buf, sizeof(buf));
                break;
            }

            case CB_READALL_PARAS: {
                for (unsigned char idx = 0; idx < DBG_COUNT_MAX; idx++) {
                    DBG_PARA_ITEM_s *item = (DBG_PARA_ITEM_s *) &ParaGroup + idx;
                    if (item->prop_s.p_s.read == 1) {
                        DBG_Report_Send(PT_App_DBG.frame_recv.Command, &item->cmd, 1);
                    }
                }

                break;
            }

            case CB_PARA_WRITE: {
                PT_PACK_DBG_u *pack = (PT_PACK_DBG_u *) PT_App_DBG.frame_recv.Data;
                for (unsigned char idx = 0; idx < DBG_COUNT_MAX; idx++) {
                    DBG_PARA_ITEM_s *item = (DBG_PARA_ITEM_s *) &ParaGroup + idx;
                    if (item->cmd == pack->para_s.cmd) {
                        if (item->value != DBG_U8Array_To_U32(pack->para_s.value)) {
                            item->value = DBG_U8Array_To_U32(pack->para_s.value);
                            DBG_Para_Mark[item->cmd] = 1;
                        }
                        DBG_Report_Send(PT_App_DBG.frame_recv.Command, &item->cmd, 1);
                    }
                }
                break;
            }
            case CB_PARA_READ: {
                PT_PACK_DBG_u *pack = (PT_PACK_DBG_u *) PT_App_DBG.frame_recv.Data;
                DBG_Report_Send(PT_App_DBG.frame_recv.Command, &pack->para_s.cmd, 1);
                break;
            }

            default:
                break;
        }
    }
}


static void DBG_Report_Send(unsigned char cmd, unsigned char *buff, unsigned char lenth) {

    PT_FRAME_s frame = {0};
    PT_PACK_DBG_u pack = {0};

    switch (cmd) {
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
            for (unsigned char idx = 0; idx < DBG_COUNT_MAX; idx++) {
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
            for (unsigned char idx = 0; idx < DBG_COUNT_MAX; idx++) {
                DBG_PARA_ITEM_s *item = (DBG_PARA_ITEM_s *) &ParaGroup + idx;
                if (item->cmd == buff[0]) {
                    pack.para_s.cmd = buff[0];
                }
            }
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
    DBG_Uart_Send_Data((unsigned char *) &frame, sizeof(frame), 20);

}


void DBG_Version_Combine(unsigned char *hw_ver, unsigned char *sw_ver, unsigned char *adv_name) {
    u_sprintf(Version_buff,
              "%s, %s, %s, PT_%s, PT_DBG_%s\n",
              hw_ver,
              sw_ver,
              adv_name,
              PT_VERSION,
              PT_DBG_VERSION);
}

void DBG_Read_Params_FromFlash(void) {
#if PARA_WRITEFLASH_ENABLE
    unsigned int lenth = DBG_COUNT_MAX * sizeof(DBG_PARA_KEY_VALUE_s);
    lenth = (lenth <= 2048 ? lenth : 2048);
    Flash_Read(FLASH_ADDR_PARAMS, KVP_Read, lenth);
#endif
}


void DBG_Write_Params_ToFlash(void) {
#if PARA_WRITEFLASH_ENABLE
    //筛选
    unsigned int offset = 0;
    for (unsigned char idx = 0; idx < DBG_COUNT_MAX; idx++) {
        DBG_PARA_ITEM_s *item = (DBG_PARA_ITEM_s *) &ParaGroup + idx;
        if (item->prop_s.p_s.flash == 1) {
            KVP_Write[offset].cmd = item->cmd;
            KVP_Write[offset].value = item->value;
            offset++;
        }
    }

    //操作Flash
    unsigned int lenth = offset * sizeof(DBG_PARA_KEY_VALUE_s);
    Flash_Write(FLASH_ADDR_PARAMS, KVP_Write, lenth);
#endif
}


unsigned int DBG_Init_Params(void) {
    unsigned int cnt = 0;
#if PARA_WRITEFLASH_ENABLE
    memset(KVP_Read, 0xff, DBG_COUNT_MAX * sizeof(DBG_PARA_KEY_VALUE_s));    //先将读缓存全置0xff

    DBG_Read_Params_FromFlash();

    for (cnt = 0; cnt < DBG_COUNT_MAX; cnt++) {
        if (KVP_Read[cnt].cmd == 0xff && KVP_Read[cnt].value == 0xffffffff) break;

        for (unsigned char idx = 0; idx < DBG_COUNT_MAX; idx++) {
            DBG_PARA_ITEM_s *item = (DBG_PARA_ITEM_s *) &ParaGroup + idx;
            if (KVP_Read[cnt].cmd == item->cmd) {
                item->value = KVP_Read[cnt].value;
            }
        }
    }
#endif
    return cnt;

}
