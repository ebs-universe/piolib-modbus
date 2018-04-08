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
 * @file dispatch.c
 * @brief Implementation of the Modbus Function Code Handler Dispatcher.
 *
 * See dispatch.h for usage documentation. Implementation 
 * documentation should be added to this file as some point. 
 * 
 * @see dispatch.h
 */

#include "dispatch.h"
#include "modbus.h"
#include "diagnostics.h"

#include "fcodes/bits.h"
#include "fcodes/regs.h"
#include "fcodes/control.h"
#include "fcodes/eit.h"
#include "fcodes/special.h"

/**
 * @name Modbus Function Code Dispatch
 */
/**@{*/ 
/**
 * \brief Recognized Modbus Function Codes and Handler Addresses
 * 
 * These are all constructed and put in place at compile time and are 
 * never copied into the RAM. This is a much less resource intensive 
 * approach compared to the AVL trees of UCDM handlers, which need to 
 * reside in RAM since the specifics are 'unknown' at compile time. 
 * 
 */
static const modbus_fcode_handler_t *modbus_fcode_handlers[]={
    &_unimpl_handler,
#if MODBUS_FRAME_UNSUPPORTED || MB_SUPPORT_FC_RD_COILS
    &_rdcoils_handler,
#endif
#if MODBUS_FRAME_UNSUPPORTED || MB_SUPPORT_FC_RD_INPUTS
    &_rdinputs_handler,
#endif
#if MODBUS_FRAME_UNSUPPORTED || MB_SUPPORT_FC_RD_HREG
    &_rdhreg_handler,
#endif
#if MODBUS_FRAME_UNSUPPORTED || MB_SUPPORT_FC_RD_IREG
    &_rdireg_handler,
#endif
#if MODBUS_FRAME_UNSUPPORTED || MB_SUPPORT_FC_WR_SCOIL
    &_wrscoil_handler,
#endif
#if MODBUS_FRAME_UNSUPPORTED || MB_SUPPORT_FC_WR_SREG
    &_wrsreg_handler,
#endif
#if MODBUS_FRAME_UNSUPPORTED || MB_SUPPORT_FC_RD_EXCST
    &_rdexcst_handler,
#endif
#if MODBUS_FRAME_UNSUPPORTED || MB_SUPPORT_FC_DIAGNOSTICS
    &_diagnostics_handler,
#endif
#if MODBUS_FRAME_UNSUPPORTED || MB_SUPPORT_FC_GT_CECNT
    &_gtcecnt_handler,
#endif
#if MODBUS_FRAME_UNSUPPORTED || MB_SUPPORT_FC_GT_CELOG
    &_gtcelog_handler,
#endif
#if MODBUS_FRAME_UNSUPPORTED || MB_SUPPORT_FC_WR_MCOILS
    &_wrmcoils_handler,
#endif
#if MODBUS_FRAME_UNSUPPORTED || MB_SUPPORT_FC_WR_MREGS
    &_wrmregs_handler,
#endif
#if MODBUS_FRAME_UNSUPPORTED || MB_SUPPORT_FC_REP_SID
    &_repsid_handler,
#endif
#if MODBUS_FRAME_UNSUPPORTED || MB_SUPPORT_FC_RD_FREC
    &_rdfrec_handler,
#endif
#if MODBUS_FRAME_UNSUPPORTED || MB_SUPPORT_FC_WR_FREC
    &_wrfrec_handler,
#endif
#if MODBUS_FRAME_UNSUPPORTED || MB_SUPPORT_FC_WR_REGM
    &_wrregm_handler,
#endif
#if MODBUS_FRAME_UNSUPPORTED || MB_SUPPORT_FC_RW_MREGS
    &_rwmregs_handler,
#endif
#if MODBUS_FRAME_UNSUPPORTED || MB_SUPPORT_FC_RD_FIFOQ    
    &_rdfifoq_handler,
#endif
#if MODBUS_FRAME_UNSUPPORTED || MB_SUPPORT_FC_EIT
    &_eit_handler,
#endif
};


static inline uint8_t _modbus_num_fcodes(void);

static inline uint8_t _modbus_num_fcodes(void){
    return (uint8_t)(sizeof(modbus_fcode_handlers) / sizeof(modbus_fcode_handlers[0]));
}

const modbus_fcode_handler_t* modbus_get_fcode_handler(uint8_t fcode){
    uint8_t low = 0;
    uint8_t high = _modbus_num_fcodes() - 1;
    uint8_t middle;
    
    // TODO Figure out if the setup overhead actually makes this _less_ efficient 
    // than a naive linear search.
    
    // TODO Consider integration into libds / reuse of libds functions.
    
    while(low <= high){
        middle = (low + high) / 2;
        if (modbus_fcode_handlers[middle]->fcode == fcode){
            return modbus_fcode_handlers[middle];
        }
        else if (modbus_fcode_handlers[middle]->fcode > fcode){
            high = middle - 1;
        }
        else{
            low = middle + 1;
        }
    }
    
    return modbus_fcode_handlers[0];
}
/**@}*/ 
