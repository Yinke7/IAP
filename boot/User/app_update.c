//
// Created by Yinke7 on 2023/12/6.
//

#include "app_update.h"

UPDATE_PARA_s Update_Para;

bool Update_Check_Normal(void) {

    Flash_Read(FLASH_UPDATE_PARA_ADDR, (uint16_t *) &Update_Para, sizeof(UPDATE_PARA_s));

    return (Update_Para.state == U_NORMAL && Update_Para.state != 0xff) ? true : false;

}