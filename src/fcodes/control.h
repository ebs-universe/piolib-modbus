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
 * @file fcode_control.h
 * @brief Modbus Control and Diagnostics Function Code Handlers.
 */


#ifndef MODBUS_FCODE_CONTROL_H
#define MODBUS_FCODE_CONTROL_H

#include <stdint.h>
#include "common.h"

void modbus_handler_rdexcst(void);
void modbus_handler_diagnostics(void);
void modbus_handler_gtcecnt(void);
void modbus_handler_gtcelog(void);

extern const modbus_fcode_handler_t _rdexcst_handler;
extern const modbus_fcode_handler_t _diagnostics_handler;
extern const modbus_fcode_handler_t _gtcecnt_handler;
extern const modbus_fcode_handler_t _gtcelog_handler;

#endif