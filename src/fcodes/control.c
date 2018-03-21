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
 * @file fcode_control.c
 * @brief Modbus Control and Diagnostics Function Code Handler Implementations.
 */

#include <ucdm/ucdm.h>

#include "../modbus.h"
#include "../dispatch.h"
#include "../diagnostics.h"
#include "control.h"


void modbus_handler_rdexcst(void){
    MODBUS_RBYTE(1) = *modbus_exception_status_p;
    modbus_sm.rxtxlen = modbus_sm.aduformat->prefix_n + 2;
    modbus_sm.aduformat->pack();
    return;
}

void modbus_handler_diagnostics(void){
    uint16_t sfcode = MODBUS_RWORD(1, 2);
    uint16_t rval;
    
    if (modbus_sm.silent && (sfcode != 0x0001)){
        modbus_sm.rxtxlen = 0;
        return;
    }
    
    switch(sfcode){
        case MB_SFC_DAIG_RETQDATA:
            break;
        case MB_SFC_DAIG_RESTCOMM:
        case MB_SFC_DAIG_CHASCDELIM:
            if(MODBUS_RBYTE(4)){
                modbus_build_exc_response(0x03);
                return;
            }
            break;
        default:
            if(MODBUS_RBYTE(3) || MODBUS_RBYTE(4)){
                modbus_build_exc_response(0x03);
                return;
            }
            break;
    }
    
    switch(sfcode){
        case MB_SFC_DAIG_FORCELISTEN:
            modbus_sm.silent = MODBUS_OUT_SILENT;
            modbus_sm.rxtxlen = 0;
            return;
        case MB_SFC_DAIG_RESTCOMM:
            if MODBUS_RBYTE(3){
                // Clear Event Log
                modbus_clear_eventlog();
            }
            if(modbus_sm.silent){
                modbus_clear_counters();
                modbus_sm.silent = MODBUS_OUT_NORMAL;
                modbus_sm.rxtxlen = 0;
                return;
            }
            modbus_sm.silent = MODBUS_OUT_NORMAL;
        case MB_SFC_DAIG_CLRCNTS:
            *modbus_diagnostic_register_p = 0x0000;
            modbus_clear_counters();
        case MB_SFC_DAIG_CLRBCHOVCNT:
            modbus_bus_char_overrun_cnt = 0;
        case MB_SFC_DAIG_RETQDATA:
        case MB_SFC_DAIG_CHASCDELIM:
            rval = MODBUS_RWORD(3, 4);
            break;
        case MB_SFC_DAIG_RETDIAGR:
            rval = *modbus_diagnostic_register_p;
            break;
        case MB_SFC_DAIG_RETBMCNT:
            rval = modbus_bus_msg_cnt;
            break;
        case MB_SFC_DAIG_RETBCECNT:
            rval = modbus_bus_comm_err_cnt;
            break;
        case MB_SFC_DAIG_RETBEECNT:
            rval = modbus_bus_exception_cnt;
            break;
        case MB_SFC_DAIG_RETSMCNT:
            rval = modbus_server_msg_cnt;
            break;
        case MB_SFC_DAIG_RETSNRCNT:
            rval = modbus_server_noresp_cnt;
            break;
        case MB_SFC_DAIG_RETSNKCNT:
            rval = modbus_server_nak_cnt;
            break;
        case MB_SFC_DAIG_RETSBUSYCNT:
            rval = modbus_server_busy_cnt;
            break;
        case MB_SFC_DAIG_RETBCHOVCNT:
            rval = modbus_bus_char_overrun_cnt;
            break;
        default:
            modbus_build_exc_response(0x01);
            return;
    }
    
    modbus_rxtxbuf[modbus_sm.aduformat->prefix_n + 3] = (uint8_t)(rval >> 8);
    modbus_rxtxbuf[modbus_sm.aduformat->prefix_n + 4] = (uint8_t)(rval);
    modbus_sm.rxtxlen = modbus_sm.aduformat->prefix_n + 5;
    modbus_sm.aduformat->pack();
    return;
}

void modbus_handler_gtcelog(void){
    // Sequential Flow Control on MODBUS interface. There can't be a previously
    // running modbus function. This can be revisited if / when the modbus state
    // machine is adapted to allow parallel processing.
    uint8_t * wp = &MODBUS_RBYTE(1);
    uint8_t llen = bytebuf_cPopulation(&modbus_comm_event_log);
    
    if (modbus_ctrans.fcode != MB_FC_GT_CECNT){
        *(wp++) = (llen + 6);
    }
    
    *(wp++) = 0x00;
    *(wp++) = 0x00;
    *(wp++) = (uint8_t)(modbus_comm_event_cnt >> 8);
    *(wp++) = (uint8_t)(modbus_comm_event_cnt);
    
    if (modbus_ctrans.fcode == MB_FC_GT_CECNT){
        modbus_sm.rxtxlen = modbus_sm.aduformat->prefix_n + 5;
        modbus_sm.aduformat->pack();
        return;
    }
    
    *(wp++) = 0x00;
    *(wp++) = llen;
    bytebuf_cCopyLen(&modbus_comm_event_log, wp, llen);
    return;
}

const modbus_fcode_handler_t _rdexcst_handler = {
    MB_FC_RD_EXCST,
    1, 0,
    #if MB_SUPPORT_FC_RD_EXCST
        &modbus_handler_rdexcst,
    #else
        &modbus_handler_notimpl,
    #endif
};

const modbus_fcode_handler_t _diagnostics_handler = {
    MB_FC_DIAGNOSTICS,
    5, 0,
    #if MB_SUPPORT_FC_DIAGNOSTICS
        &modbus_handler_diagnostics,
    #else
        &modbus_handler_notimpl,
    #endif
};

const modbus_fcode_handler_t _gtcecnt_handler = {
    MB_FC_GT_CECNT,
    1, 0,
    #if MB_SUPPORT_FC_GT_CECNT
        &modbus_handler_gtcelog,
    #else
        &modbus_handler_notimpl,
    #endif
};

const modbus_fcode_handler_t _gtcelog_handler = {
    MB_FC_GT_CELOG,
    1, 0,
    #if MB_SUPPORT_FC_GT_CELOG
        &modbus_handler_gtcelog,
    #else
        &modbus_handler_notimpl,
    #endif
};
