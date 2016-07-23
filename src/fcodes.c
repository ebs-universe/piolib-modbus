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

#include "modbus.h"
#include "diagnostics.h"
#include "fcodes.h"

/**
 * @name Per Command Function Code Handler Implementations
 */
/**@{*/ 

void modbus_handler_notimpl(void);
uint8_t modbus_crlen_notimpl(void);
uint8_t modbus_validator_notimpl(void);

void modbus_handler_notimpl(void){
    ;   
}

uint8_t modbus_crlen_notimpl(void){
    return -1;
}

uint8_t modbus_validator_notimpl(void){
    return 1;
}


static const modbus_fcode_handler_t _rdcoils_handler = {
    MB_FC_RD_COILS,
    &modbus_handler_notimpl,
    &modbus_crlen_notimpl,
    &modbus_validator_notimpl,
};

static const modbus_fcode_handler_t _rdinputs_handler = {
    MB_FC_RD_INPUTS,
    &modbus_handler_notimpl,
    &modbus_crlen_notimpl,
    &modbus_validator_notimpl,
};
/**@}*/ 


/**
 * @name Modbus Function Code Dispatch
 */
/**@{*/ 
/**
 * \brief Supported Modbus Function Codes and Handler Addresses
 * 
 */
static const modbus_fcode_handler_t *modbus_fcode_handlers[]={
    &_rdcoils_handler,
    &_rdinputs_handler
};


static inline uint8_t _modbus_num_fcodes(void);

static inline uint8_t _modbus_num_fcodes(void){
    return (uint8_t)(sizeof(modbus_fcode_handlers) / sizeof(modbus_fcode_handlers[0]));
}

const modbus_fcode_handler_t* modbus_get_fcode_handler(uint8_t fcode){
    return modbus_fcode_handlers[0];
}
/**@}*/ 
