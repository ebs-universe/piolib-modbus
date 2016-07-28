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
 * @file fcode_regs.h
 * @brief Holding and Input Register (Register Access) Function Code Handlers.
 */


#ifndef MODBUS_FCODE_REGS_H
#define MODBUS_FCODE_REGS_H

#include <stdint.h>
#include "common.h"

void modbus_handler_rdregs(void);
void modbus_handler_wrsreg(void);
void modbus_handler_wrregs(void);
void modbus_handler_wrregm(void);
void modbus_handler_rwmregs(void);

uint8_t modbus_crlen_wrmregs(void);
uint8_t modbus_crlen_rwmregs(void);

extern const modbus_fcode_handler_t _rdhreg_handler;
extern const modbus_fcode_handler_t _rdireg_handler;
extern const modbus_fcode_handler_t _wrsreg_handler;
extern const modbus_fcode_handler_t _wrmregs_handler;
extern const modbus_fcode_handler_t _wrregm_handler;
extern const modbus_fcode_handler_t _rwmregs_handler;

#endif