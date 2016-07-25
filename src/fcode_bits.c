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
 * @file fcode_rdbits.c
 * @brief Read Coils and Inputs (Bit Read) Function Code Handler Implementations.
 */

#include "modbus.h"
#include "diagnostics.h"
#include "fcodes.h"
#include "fcode_bits.h"


void modbus_handler_rdbits(void)
{
    modbus_handler_notimpl();
}

void modbus_handler_wrsbit(void)
{
    modbus_handler_notimpl();
}

void modbus_handler_wrbits(void)
{
    modbus_handler_notimpl();
}

const modbus_fcode_handler_t _rdcoils_handler = {
    MB_FC_RD_COILS,
    &modbus_handler_rdbits,
    &modbus_crlen_5b,
    &modbus_validator_notimpl,
};

const modbus_fcode_handler_t _rdinputs_handler = {
    MB_FC_RD_INPUTS,
    &modbus_handler_rdbits,
    &modbus_crlen_5b,
    &modbus_validator_notimpl,
};

const modbus_fcode_handler_t _wrscoil_handler = {
    MB_FC_WR_SCOIL,
    &modbus_handler_wrsbit,
    &modbus_crlen_5b,
    &modbus_validator_notimpl,
};

const modbus_fcode_handler_t _wrmcoils_handler = {
    MB_FC_WR_MCOILS,
    &modbus_handler_wrbits,
    &modbus_crlen_notimpl,
    &modbus_validator_notimpl,
};

