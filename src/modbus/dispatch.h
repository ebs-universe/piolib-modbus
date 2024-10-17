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
 * @file dispatch.h
 * @brief Interface and usage of the Modbus Function Code Dispatcher.
 * 
 * @see dispatch.c
 */


#ifndef MODBUS_DISPATCH_H
#define MODBUS_DISPATCH_H

#include <stdint.h>
#include "config.h"
#include "fcodes/common.h"

/**
 * @name Modbus Function Codes
 */
/**@{*/ 
typedef enum {
    MB_FC_RD_COILS     = 0x01,
    MB_FC_RD_INPUTS    = 0x02,
    MB_FC_RD_HREG      = 0x03,
    MB_FC_RD_IREG      = 0x04,
    MB_FC_WR_SCOIL     = 0x05,
    MB_FC_WR_SREG      = 0x06,
    MB_FC_RD_EXCST     = 0x07,
    MB_FC_DIAGNOSTICS  = 0x08,

    MB_FC_GT_CECNT     = 0x0B,
    MB_FC_GT_CELOG     = 0x0C,
    MB_FC_WR_MCOILS    = 0x0F,
    MB_FC_WR_MREGS     = 0x10,

    MB_FC_REP_SID      = 0x11,
    MB_FC_RD_FREC      = 0x14,
    MB_FC_WR_FREC      = 0x15,
    MB_FC_WR_REGM      = 0x16,
    MB_FC_RW_MREGS     = 0x17,
    MB_FC_RD_FIFOQ     = 0x18,

    MB_FC_EIT          = 0x2B,
} MODBUS_FCODE_t;

typedef enum {
    MB_SFC_EIT_CANOPEN = 0x0D,
    MB_SFC_EIT_RDDEVID = 0x0E
} MODBUS_SFCODE_EIT_t;

typedef enum {
      MB_SFC_DAIG_RETQDATA    = 0x0000,
      MB_SFC_DAIG_RESTCOMM    = 0x0001,
      MB_SFC_DAIG_RETDIAGR    = 0x0002,
      MB_SFC_DAIG_CHASCDELIM  = 0x0003,
      MB_SFC_DAIG_FORCELISTEN = 0x0004,
      MB_SFC_DAIG_CLRCNTS     = 0x000A,
      MB_SFC_DAIG_RETBMCNT    = 0x000B,
      MB_SFC_DAIG_RETBCECNT   = 0x000C,
      MB_SFC_DAIG_RETBEECNT   = 0x000D,
      MB_SFC_DAIG_RETSMCNT    = 0x000E,
      MB_SFC_DAIG_RETSNRCNT   = 0x000F,
      MB_SFC_DAIG_RETSNKCNT   = 0x0010,
      MB_SFC_DAIG_RETSBUSYCNT = 0x0011,
      MB_SFC_DAIG_RETBCHOVCNT = 0x0012,
      MB_SFC_DAIG_CLRBCHOVCNT = 0x0014
} MODBUS_SFCODE_DIAG_t;         
/**@}*/ 

/**
 * @name Modbus Function Code Handler Management and Dispatch
 * 
 */
/**@{*/ 
const modbus_fcode_handler_t* modbus_get_fcode_handler(uint8_t fcode);
/**@}*/ 

#endif