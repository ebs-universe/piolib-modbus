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
 * @file fcode_common.c
 * @brief Common Function Code Handler Implementations.
 *
 * See fcode_common.h for usage documentation. Implementation 
 * documentation should be added to this file as some point. 
 * 
 * @see fcode_common.h
 */

#include "modbus.h"
#include "diagnostics.h"
#include "fcodes.h"
#include "fcode_common.h"


void modbus_handler_notimpl(void){
    modbus_rxtxbuf[modbus_sm.aduformat->prefix_n] +=  0x80;
    modbus_rxtxbuf[modbus_sm.aduformat->prefix_n + 1] = 0x01;
    modbus_sm.rxtxlen = modbus_sm.aduformat->prefix_n + 2;
    modbus_sm.aduformat->pack();
    return;
}

uint8_t modbus_crlen_notimpl(void){
    return 0;
}

uint8_t modbus_crlen_5b(void){
    return modbus_sm.aduformat->padding_n + 5 - modbus_sm.rxtxlen;
}

uint8_t modbus_crlen_1b(void){
    return modbus_sm.aduformat->padding_n + 1 - modbus_sm.rxtxlen;
}

uint8_t modbus_crlen_7b(void){
    return modbus_sm.aduformat->padding_n + 7 - modbus_sm.rxtxlen;
}

uint8_t modbus_validator_notimpl(void){
    return 1;
}

void modbus_build_exc_response(uint8_t ecode){
    if (modbus_ctrans.broadcast || modbus_sm.silent){
        modbus_sm.rxtxlen = 0;
        return;   
    }
    modbus_rxtxbuf[modbus_sm.aduformat->prefix_n] += 0x80;
    modbus_rxtxbuf[modbus_sm.aduformat->prefix_n + 1] = ecode;
    modbus_sm.rxtxlen = modbus_sm.aduformat->prefix_n + 2;;
    modbus_sm.aduformat->pack();
    return;
}

const modbus_fcode_handler_t _unimpl_handler = {
    0x00,
    &modbus_handler_notimpl,
    &modbus_crlen_notimpl,
    &modbus_validator_notimpl,
};
