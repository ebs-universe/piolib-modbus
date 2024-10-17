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
 * @file fcode_regs.c
 * @brief Register Function Codes Handler Implementations.
 */

#include <ucdm/ucdm.h>

#include "../modbus.h"
#include "../dispatch.h"
#include "../diagnostics.h"
#include "regs.h"

// TODO
// It _may_ be possible to refactor these functions to reduce code duplication
// and reduce overall code foofprint. This will, however, deepen the worst case 
// call stack.

void modbus_handler_rdregs(void)
{
    uint8_t n = MODBUS_RBYTE(4);
    uint16_t saddr = MODBUS_RWORD(1, 2);
    uint16_t tvar;
    uint8_t * wp;
    
    if ((saddr + n >= UCDM_MAX_REGISTERS)){    
        modbus_build_exc_response(0x02);
        return;
    }
    
    if (modbus_ctrans.broadcast){
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
    uint8_t res = ucdm_set_register(MODBUS_RWORD(1, 2), 
                                    MODBUS_RWORD(3, 4));
    switch(res){
        case(0x01):
            modbus_build_exc_response(0x02);
            break;
        case(0x02):
            modbus_build_exc_response(0x04);
            break;
        default:
            modbus_sm.aduformat->pack();
            break;
    }
    return;
}

void modbus_handler_wrregs(void)
{
    uint8_t n = MODBUS_RBYTE(4);
    uint16_t saddr = MODBUS_RWORD(1, 2);
    uint16_t tvar;
    uint8_t * rp;
    uint8_t rval;
    uint8_t scount = 0;
    
    if (saddr + n >= UCDM_MAX_REGISTERS){    
        // WARNING If some / all of the registers are not writeable, 
        // an exception is not returned. This is unavoidable because
        // errors would show up only after some of the registers have 
        // already been written, leaving an indeterminate state. This
        // way, we hold the application responsible. 
        modbus_build_exc_response(0x02);
        return;
    }
    
    rp = &MODBUS_RBYTE(6);
       
    for(uint8_t i=0; i<n; i++){
        tvar = (uint16_t)(*(rp++));
        tvar = tvar << 8;
        tvar |= *(rp++);
        rval = ucdm_set_register(saddr + i, tvar);
        if (rval == 0) scount ++;
    }
    
    if (modbus_ctrans.broadcast){
        return;   
    }
    modbus_sm.rxtxlen = modbus_sm.aduformat->prefix_n + 5;
    MODBUS_RBYTE(4) = scount;
    modbus_sm.aduformat->pack();
    return;
}

void modbus_handler_wrregm(void)
{       
    uint16_t addr =     MODBUS_RWORD(1, 2);
    uint16_t and_mask = MODBUS_RWORD(3, 4);
    uint16_t or_mask =  MODBUS_RWORD(5, 6);
    uint16_t tvar;
    uint8_t res;
    
    if (addr >= UCDM_MAX_REGISTERS){    
        modbus_build_exc_response(0x02);
        return;
    }
    
    tvar = ucdm_get_register(addr);
    tvar = (tvar & and_mask) | (or_mask & (!and_mask));
    res = ucdm_set_register(addr, tvar);
    
    if(res){
        modbus_build_exc_response(0x02);
        return;
    }
    return;
}

void modbus_handler_rwmregs(void)
{
    uint8_t nr = MODBUS_RBYTE(4);
    uint16_t rsaddr = MODBUS_RWORD(1, 2);
    uint8_t nw = MODBUS_RBYTE(8);
    uint16_t wsaddr = MODBUS_RWORD(5, 6);
    uint8_t * ap;
    uint16_t tvar;
    
    if ((rsaddr + nr > UCDM_MAX_REGISTERS) || (wsaddr + nr > UCDM_MAX_REGISTERS)){
        modbus_build_exc_response(0x02);
        return;
    }
    
    ap = &MODBUS_RBYTE(10);
       
    for(uint8_t i=0; i<=nw; i++){
        tvar = (uint16_t)(*(ap++));
        tvar = tvar << 8;
        tvar |= *(ap++);
        ucdm_set_register(wsaddr + i, tvar);
    }
    
    if (modbus_ctrans.broadcast){
        return;   
    }
    
    ap = &MODBUS_RBYTE(1);
    *(ap++) = 2 * nr;
       
    for(uint8_t i=0; i<=nr; i++){
        tvar = ucdm_get_register(rsaddr + i);
        *(ap++) = (uint8_t)(tvar >> 8);
        *(ap++) = (uint8_t)(tvar);
    }
    
    modbus_sm.rxtxlen = modbus_sm.aduformat->prefix_n + 2 + 2*nr;
    modbus_sm.aduformat->pack();
    return;
}

const modbus_fcode_handler_t _rdhreg_handler = {
    MB_FC_RD_HREG,
    5, 0,
    #if MB_SUPPORT_FC_RD_HREG
        &modbus_handler_rdregs,
    #else
        &modbus_handler_notimpl,
    #endif
};

const modbus_fcode_handler_t _rdireg_handler = {
    MB_FC_RD_IREG,
    5, 0,
    #if MB_SUPPORT_FC_RD_IREG
        &modbus_handler_rdregs,
    #else
        &modbus_handler_notimpl,
    #endif
};

const modbus_fcode_handler_t _wrsreg_handler = {
    MB_FC_WR_SREG,
    5, 0,
    #if MB_SUPPORT_FC_WR_SREG
        &modbus_handler_wrsreg,
    #else
        &modbus_handler_notimpl,
    #endif
};

const modbus_fcode_handler_t _wrmregs_handler = {
    MB_FC_WR_MREGS,
    6, 5,
    #if MB_SUPPORT_FC_WR_MREGS
        &modbus_handler_wrregs,
    #else
        &modbus_handler_notimpl,
    #endif
};

const modbus_fcode_handler_t _wrregm_handler = {
    MB_FC_WR_REGM,
    7, 0,
    #if MB_SUPPORT_FC_WR_REGM
        &modbus_handler_wrregm,
    #else
        &modbus_handler_notimpl,
    #endif
};

const modbus_fcode_handler_t _rwmregs_handler = {
    MB_FC_RW_MREGS,
    10, 9,
    #if MB_SUPPORT_FC_RW_MREGS
        &modbus_handler_rwmregs,
    #else
        &modbus_handler_notimpl,
    #endif
};
