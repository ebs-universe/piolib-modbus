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
 * @file fcodes/bits.c
 * @brief Read Coils and Inputs (Bit Read) Function Code Handler Implementations.
 */

#include <ucdm/ucdm.h>

#include "../modbus.h"
#include "../diagnostics.h"
#include "../dispatch.h"
#include "bits.h"


void modbus_handler_rdbits(void)
{
    uint16_t addrb = MODBUS_RWORD(1, 2);
    uint16_t len = MODBUS_RWORD(3, 4);
    uint8_t res, byte, mask;
    uint8_t * wp;
    
    if (len > 0x07D0){
        modbus_build_exc_response(0x03);
        return;
    }
    if (addrb + len > UCDM_MAX_BITS){
        modbus_build_exc_response(0x02);
        return;
    }
    
    wp = &modbus_rxtxbuf[modbus_sm.aduformat->prefix_n + 1];
    uint8_t olen = len / 8 + (len % 8 != 0);
    modbus_sm.rxtxlen = modbus_sm.aduformat->prefix_n + 2 + olen;
    *(wp++) = olen;
    
    byte = 0x00;
    mask = 0x01;
    while (len){
        res = ucdm_get_bit(addrb);
        if (res) byte |= mask;
        if (mask != 0x80){
            mask = mask << 1; 
        }
        else{
            *(wp++) = byte;
            byte = 0x00;
            mask = 0x01;
        }
        len--;
        addrb++;
    }
    if (mask != 0x01){
        *(wp++) = byte;
    }
    
    modbus_sm.aduformat->pack();
    return;
}

void modbus_handler_wrsbit(void)
{
    uint16_t addrb = MODBUS_RWORD(1, 2);
    uint8_t val = MODBUS_RBYTE(3);
    uint8_t res;
    
    if (val){
        res = ucdm_set_bit(addrb);
    }
    else{
        res = ucdm_clear_bit(addrb);
    }
    if (res){
        modbus_build_exc_response(0x02);
        return;
    }
    return;
}

void modbus_handler_wrbits(void)
{
    uint16_t addrb = MODBUS_RWORD(1, 2);
    uint16_t len = MODBUS_RWORD(3, 4);
    uint16_t scount = 0;
    uint8_t res, bit, mask;
    uint8_t * rp = &MODBUS_RBYTE(6);
    
    if (len > 0x07D0){
        modbus_build_exc_response(0x03);
        return;
    }
    if (addrb + len > UCDM_MAX_BITS){
        modbus_build_exc_response(0x02);
        return;
    }
    
    mask = 0x01;
    while(len){
        bit = *rp | mask;
        if (bit){
            res = ucdm_set_bit(addrb);
        }
        else{
            res = ucdm_clear_bit(addrb);
        }
        if (res == 0){
            scount ++;
        }
        addrb ++;
        len --;
        if (mask == 0x80){
            mask = 0x01;
            rp ++;
        }
        else{
            mask = mask << 1;
        }
    }
    
    MODBUS_RBYTE(3) = (uint8_t)(scount >> 8);
    MODBUS_RBYTE(4) = (uint8_t)(scount);
    modbus_sm.rxtxlen = modbus_sm.aduformat->prefix_n + 5;
    modbus_sm.aduformat->pack();
    return;
}

const modbus_fcode_handler_t _rdcoils_handler = {
    MB_FC_RD_COILS,
    5, 0,
    #if MB_SUPPORT_FC_RD_COILS
        &modbus_handler_rdbits,
    #else
        &modbus_handler_notimpl,
    #endif
};

const modbus_fcode_handler_t _rdinputs_handler = {
    MB_FC_RD_INPUTS,
    5, 0,
    #if MB_SUPPORT_FC_RD_INPUTS
        &modbus_handler_rdbits,
    #else
        &modbus_handler_notimpl,
    #endif
};

const modbus_fcode_handler_t _wrscoil_handler = {
    MB_FC_WR_SCOIL,
    5, 0,
    #if MB_SUPPORT_FC_WR_SCOIL
        &modbus_handler_wrsbit,
    #else
        &modbus_handler_notimpl,
    #endif
};

const modbus_fcode_handler_t _wrmcoils_handler = {
    MB_FC_WR_MCOILS,
    0, 0,
    #if MB_SUPPORT_FC_WR_MCOILS
        &modbus_handler_wrbits,
    #else
        &modbus_handler_notimpl,
    #endif
};

