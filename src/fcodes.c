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
 * @file fcodes.c
 * @brief Implementation of the Modbus Function Code Handlers.
 *
 * See fcodes.h for usage documentation. Implementation 
 * documentation should be added to this file as some point. 
 * 
 * @see fcodes.h
 */

#include "fcodes.h"
#include "modbus.h"
#include "diagnostics.h"

#include "fcode_bits.h"
#include "fcode_regs.h"
#include "fcode_control.h"
#include "fcode_special.h"

/**
 * @name Modbus Function Code Dispatch
 */
/**@{*/ 
/**
 * \brief Recognized Modbus Function Codes and Handler Addresses
 * 
 * TODO Consider replacing with a proper binary search tree?
 * 
 */
static const modbus_fcode_handler_t *modbus_fcode_handlers[]={
    &_unimpl_handler,
    &_rdcoils_handler,
    &_rdinputs_handler,
    &_rdhreg_handler,
    &_rdireg_handler,
    &_wrscoil_handler,
    &_wrsreg_handler,
    &_rdexcst_handler,
    &_diagnostics_handler,
    &_gtcecnt_handler,
    &_gtcelog_handler,
    &_wrmcoils_handler,
    &_wrmregs_handler,
    &_repsid_handler,
    &_rdfrec_handler,
    &_wrfrec_handler,
    &_wrregm_handler,
    &_rwmregs_handler,
    &_rdfifoq_handler,
    &_eit_handler,
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
