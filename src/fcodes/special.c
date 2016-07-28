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
 * @file fcode_special.c
 * @brief Special Function Code Handler Implementations.
 */

#include <ucdm/ucdm.h>

#include "../modbus.h"
#include "../dispatch.h"
#include "special.h"

void modbus_handler_repsid(void)
{
    modbus_handler_notimpl();
}

void modbus_handler_rdfrec(void)
{
    modbus_handler_notimpl();
}

void modbus_handler_wrfrec(void)
{
    modbus_handler_notimpl();
}

void modbus_handler_fifoq(void)
{
    modbus_handler_notimpl();
}

void modbus_handler_eit(void)
{
    modbus_handler_notimpl();
}

const modbus_fcode_handler_t _repsid_handler = {
    MB_FC_REP_SID,
    1, 0,
    #if MB_SUPPORT_FC_REP_SID
        &modbus_handler_repsid,
    #else
        &modbus_handler_notimpl,
    #endif
};

const modbus_fcode_handler_t _rdfrec_handler = {
    MB_FC_RD_FREC,
    2, 1,
    #if MB_SUPPORT_FC_RD_FREC
        &modbus_handler_rdfrec,
    #else
        &modbus_handler_notimpl,
    #endif
};

const modbus_fcode_handler_t _wrfrec_handler = {
    MB_FC_WR_FREC,
    2, 1,
    #if MB_SUPPORT_FC_WR_FREC
        &modbus_handler_wrfrec,
    #else
        &modbus_handler_notimpl,
    #endif
};

const modbus_fcode_handler_t _rdfifoq_handler = {
    MB_FC_RD_FIFOQ,
    3, 0,
    #if MB_SUPPORT_FC_RD_FIFOQ
        &modbus_handler_fifoq,
    #else
        &modbus_handler_notimpl,
    #endif
};

const modbus_fcode_handler_t _eit_handler = {
    MB_FC_EIT,
    0, 0,
    #if MB_SUPPORT_FC_EIT
        &modbus_handler_eit,
    #else
        &modbus_handler_notimpl,
    #endif
};
