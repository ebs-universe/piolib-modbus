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

#include "../modbus.h"
#include "../diagnostics.h"
#include "../dispatch.h"
#include "common.h"


void modbus_handler_notimpl(void){
    modbus_build_exc_response(0x01);
    return;
}


void modbus_build_exc_response(uint8_t ecode){
    #if MB_DIAGNOSTICS
        modbus_bus_exception_cnt ++;
        #if MB_SUPPORT_CELOGFUNCS
        uint8_t event=0x00;
        switch(ecode){
            case 0x01:
            case 0x02:
            case 0x03:
                event = 1<<0;
                break;
            case 0x04:
                event = 1<<1;
                break;
            case 0x05:
            case 0x06:
                event = 1<<2;
                break;
            case 0x07:
                event = 1<<3;
        }
        modbus_log_sent(event);
        #endif
    #endif
    if (modbus_ctrans.broadcast){
        return;
    }
    #if MB_DIAGNOSTICS
    switch(ecode){
        case 0x07:
            modbus_server_nak_cnt ++;
            break;
        case 0x06:
            modbus_server_busy_cnt ++;
            break;
    }
    #endif
    modbus_rxtxbuf[modbus_sm.aduformat->prefix_n] += 0x80;
    modbus_rxtxbuf[modbus_sm.aduformat->prefix_n + 1] = ecode;
    modbus_sm.rxtxlen = modbus_sm.aduformat->prefix_n + 2;
    modbus_sm.aduformat->pack();
    return;
}

const modbus_fcode_handler_t _unimpl_handler = {
    0x00,
    0, 0,
    &modbus_handler_notimpl,
};
