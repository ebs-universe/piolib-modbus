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
 * @file fcode_rdregs.c
 * @brief Read Holding and Input Register (Register Read) Function Code Handler Implementations.
 */

#include <string.h>
#include <ucdm/ucdm.h>

#include "modbus.h"
#include "diagnostics.h"
#include "fcodes.h"
#include "fcode_regs.h"

// NOTE 
// UCDM expects uint8_t register addresses. Leave that cast to the very 
// last step to simplify conversion later on, if / when UCDM is moved to 
// uint16_t addresses.

void modbus_handler_rdregs(void)
{
    uint8_t n = MODBUS_RBYTE(4);
    uint16_t saddr = MODBUS_RWORD(1, 2) - 1;
    uint16_t tvar;
    uint8_t * wp;
    
    if (saddr + n >= DMAP_MAXREGS){    
        modbus_build_exc_response(0x02);
        return;
    }
    
    if (modbus_ctrans.broadcast || modbus_sm.silent){
        modbus_sm.rxtxlen = 0;
        return;
    }
    
    wp = &modbus_rxtxbuf[modbus_sm.aduformat->prefix_n + 1];
    *(wp++) = 2 * n;
       
    for(uint8_t i=0; i<=n; i++){
        tvar = ucdm_get_register(saddr + i);
        *(wp++) = (uint8_t)(tvar >> 8);
        *(wp++) = (uint8_t)(tvar);
    }
    
    modbus_sm.rxtxlen = modbus_sm.aduformat->prefix_n + 2 + 2*n;
    modbus_sm.aduformat->pack();
    return;
}

void modbus_handler_wrsreg(void)
{
    uint8_t res = ucdm_set_register((uint8_t)(MODBUS_RWORD(1, 2) - 1), MODBUS_RWORD(3, 4));
    if (res){
        modbus_build_exc_response(0x02);
        return;
    }
    if (modbus_ctrans.broadcast || modbus_sm.silent){
        modbus_sm.rxtxlen = 0;
        return;   
    }
}

void modbus_handler_wrregs(void)
{
    uint8_t n = MODBUS_RBYTE(4);
    uint16_t saddr = MODBUS_RWORD(1, 2) - 1;
    uint16_t tvar;
    uint8_t * rp;
    uint8_t scount=0;
    
    if (saddr + n >= DMAP_MAXREGS){    
        // WARNING If some / all of the registers are not writeable, 
        // an exception is not returned.   
        modbus_build_exc_response(0x02);
        return;
    }
    
    rp = &MODBUS_RBYTE(6);
       
    for(uint8_t i=0; i<=n; i++){
        tvar = (uint16_t)(*(rp++));
        tvar = tvar << 8;
        tvar |= *(rp++);
        scount += ucdm_set_register((uint8_t)(saddr + i), tvar);
    }
    
    if (modbus_ctrans.broadcast || modbus_sm.silent){
        modbus_sm.rxtxlen = 0;
        return;   
    }
    modbus_sm.rxtxlen = modbus_sm.aduformat->prefix_n + 5;
    MODBUS_RBYTE(4) = scount;
    modbus_sm.aduformat->pack();
    return;
}

void modbus_handler_wrregm(void)
{       
    uint16_t addr =     MODBUS_RWORD(1, 2) - 1;
    uint16_t and_mask = MODBUS_RWORD(3, 4);
    uint16_t or_mask =  MODBUS_RWORD(5, 6);
    uint16_t tvar;
    uint8_t res;
    
    if (addr >= DMAP_MAXREGS){    
        modbus_build_exc_response(0x02);
        return;
    }
    
    tvar = ucdm_get_register((uint8_t)addr);
    tvar = (tvar & and_mask) | (or_mask & (!and_mask));
    res = ucdm_set_register((uint8_t)addr, tvar);
    
    if(res){
        modbus_build_exc_response(0x02);
        return;
    }
    
    if (modbus_ctrans.broadcast || modbus_sm.silent){
        modbus_sm.rxtxlen = 0;
        return;   
    }
    
    return;
}

void modbus_handler_rwmregs(void)
{
    modbus_handler_notimpl();
}

const modbus_fcode_handler_t _rdhreg_handler = {
    MB_FC_RD_HREG,
    &modbus_handler_rdregs,
    &modbus_crlen_5b,
    &modbus_validator_notimpl,
};

const modbus_fcode_handler_t _rdireg_handler = {
    MB_FC_RD_IREG,
    &modbus_handler_rdregs,
    &modbus_crlen_5b,
    &modbus_validator_notimpl,
};

const modbus_fcode_handler_t _wrsreg_handler = {
    MB_FC_WR_SREG,
    &modbus_handler_wrsreg,
    &modbus_crlen_5b,
    &modbus_validator_notimpl,
};

const modbus_fcode_handler_t _wrmregs_handler = {
    MB_FC_WR_MREGS,
    &modbus_handler_wrregs,
    &modbus_crlen_notimpl,
    &modbus_validator_notimpl,
};

const modbus_fcode_handler_t _wrregm_handler = {
    MB_FC_WR_REGM,
    &modbus_handler_wrregm,
    &modbus_crlen_7b,
    &modbus_validator_notimpl,
};

const modbus_fcode_handler_t _rwmregs_handler = {
    MB_FC_RW_MREGS,
    &modbus_handler_rwmregs,
    &modbus_crlen_notimpl,
    &modbus_validator_notimpl,
};

