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
 * @file fcodes/crlen.h
 * @brief MODBUS Ccommand Length Calculator
 * 
 * Calculates the residual command lengths for MODBUS commands in a 
 * manner that allows for command content based framing.
 * 
 * @see <http://www.modbus.org/docs/Modbus_Application_Protocol_V1_1b3.pdf>
 * @see fcodes/crlen.c
 */


#ifndef MODBUS_FCODE_CRLEN_H
#define MODBUS_FCODE_CRLEN_H

#include <stdint.h>

uint8_t modbus_crlen(void);

/**@}*/ 

#endif
