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
 * @file fcode_common.h
 * @brief Common Function Code Handlers.
 * 
 * @see <http://www.modbus.org/docs/Modbus_Application_Protocol_V1_1b3.pdf>
 * @see fcode_common.c
 */


#ifndef MODBUS_FCODE_COMMON_H
#define MODBUS_FCODE_COMMON_H

#include <stdint.h>
#define MODBUS_RBYTE(a)    ( modbus_rxtxbuf[(modbus_sm.aduformat->prefix_n) + a] )
#define MODBUS_RWORD(a, b) (((uint16_t)(MODBUS_RBYTE(a)) << 8) | (MODBUS_RBYTE(b)))

typedef struct {
    const uint8_t fcode;
    const uint8_t apriorilen;
    const uint8_t addlen_idx;
    void (*const handler)(void);
} modbus_fcode_handler_t;

void modbus_handler_notimpl(void);
uint8_t modbus_crlen(void);
void modbus_build_exc_response(uint8_t ecode);

extern const modbus_fcode_handler_t _unimpl_handler;

/**@}*/ 

#endif