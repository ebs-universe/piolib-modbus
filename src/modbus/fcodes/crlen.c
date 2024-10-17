/* 
   Copyright (c)
     (c) 2015-2016 Chintalagiri Shashank, Quazar Technologies Pvt. Ltd.
   
   This file is part of
   Embedded bootstraps : modbus library
   
   This library is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published
   by the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>. 
*/

/**
 * @file fcode_crlen.c
 * @brief MODBUS Ccommand Length Calculator
 *
 * See fcode_crlen.h for usage documentation. Implementation 
 * documentation should be added to this file as some point. 
 * 
 * @see fcode_crlen.h
 */

#include "../modbus.h"
#include "../dispatch.h"
#include "crlen.h"


static uint8_t _modbus_crlen_eit(void);

static uint8_t _modbus_crlen_eit_rddevid(void){
    return(2);
}


static uint8_t _modbus_crlen_eit(void){
    uint8_t meitype = MODBUS_RBYTE(1);
    switch(meitype){
        case(MB_SFC_EIT_RDDEVID):
            return (_modbus_crlen_eit_rddevid());
        default:
            return(0);
    }
}


uint8_t modbus_crlen(void){
    uint8_t apriorilen = modbus_ctrans.fcode_handler->apriorilen;
    uint8_t addllen = 0;
    if (!apriorilen){
        return 0;
    }
    apriorilen += modbus_sm.aduformat->prefix_n;
    if (modbus_sm.rxtxlen < apriorilen){
        return(apriorilen - modbus_sm.rxtxlen);
    }
    else{
        if (modbus_ctrans.fcode_handler->addlen_idx){
            addllen = MODBUS_RBYTE(modbus_ctrans.fcode_handler->addlen_idx);
        }
        #if MB_SUPPORT_FC_EIT
        else if (modbus_ctrans.fcode_handler->fcode == MB_FC_EIT){
            addllen = _modbus_crlen_eit();
        }
        #endif
        return(apriorilen + addllen + modbus_sm.aduformat->postfix_n - modbus_sm.rxtxlen);
    }
}
