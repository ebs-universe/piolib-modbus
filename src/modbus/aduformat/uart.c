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
 * @file aduformat/uart.c
 * @brief Implementation of the Modbus library.
 *
 * See modbus.h for usage documentation. Implementation 
 * documentation should be added to this file as some point. 
 * bytebuf
 * @see aduformat/uart.h
 */

#include <ucdm/ucdm.h>

#include "../modbus.h"
#include "../interface.h"
#include "../diagnostics.h"
#include "uart.h"
#include "crc.h"


const modbus_aduformat_t modbus_aduformat_uart = {
    1, 
    2,
    &modbus_uart_adu_pack,
    &modbus_uart_adu_validate,
    &modbus_uart_adu_write,
};

HAL_BASE_t modbus_uart_adu_validate(void){
    //Get Address included in message
    uint8_t apu_addr = modbus_rxtxbuf[0];
    //Get CRC included in message
    uint16_t apu_crc = (modbus_rxtxbuf[modbus_sm.rxtxlen-2]) | ((uint16_t)(modbus_rxtxbuf[modbus_sm.rxtxlen-1])<<8);
    // Calculate CRC of message
    uint16_t crc = modbus_calculate_crc(&(modbus_rxtxbuf[0]), (modbus_sm.rxtxlen-2));
    
    if (apu_crc != crc){
        #if MB_DIAGNOSTICS
        modbus_bus_comm_err_cnt ++;
        modbus_log_received(1<<1);
        #endif
        return 0;
    }
    
    #if MB_DIAGNOSTICS
    modbus_bus_msg_cnt ++;
    #endif
    
    if (!apu_addr){
        modbus_ctrans.broadcast = MODBUS_CTT_BROADCAST;
    }
    else if(apu_addr == modbus_control.address){
        modbus_ctrans.broadcast = MODBUS_CTT_UNICAST;
    }
    else{
        // Not addressed to us
        return 0;
    }
    
    return 1;
}

void modbus_uart_adu_pack(void){
    uint16_t crc;
    crc = modbus_calculate_crc(&modbus_rxtxbuf[0], 
                               modbus_sm.rxtxlen);
    modbus_rxtxbuf[modbus_sm.rxtxlen] = (uint8_t)crc;
    modbus_rxtxbuf[modbus_sm.rxtxlen+1] = (uint8_t)(crc >> 8);
    modbus_sm.rxtxlen += 2;
}

void modbus_uart_adu_write(void){
    uint8_t tvar8 = modbus_if_reqlock(0);
    if (tvar8 >= modbus_sm.rxtxlen){
        tvar8 = modbus_sm.rxtxlen;
    }
    else if(tvar8 > MODBUS_TXCHUNKSIZE){
        tvar8 = MODBUS_TXCHUNKSIZE;
    }
    else{
        tvar8 = 0;
    }
    if(tvar8){
        modbus_if_write(modbus_sm.txp, tvar8);
        modbus_sm.rxtxlen -= tvar8;
        modbus_sm.txp += tvar8;
    }
    if(!modbus_sm.rxtxlen){
        modbus_if_flush();
    }
    modbus_if_rellock();
    return;
}
