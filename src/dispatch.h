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
#define MB_FC_RD_COILS                0x01
#define MB_FC_RD_INPUTS               0x02
#define MB_FC_RD_HREG                 0x03
#define MB_FC_RD_IREG                 0x04
#define MB_FC_WR_SCOIL                0x05
#define MB_FC_WR_SREG                 0x06
#define MB_FC_RD_EXCST                0x07
                                      
#define MB_FC_GT_CECNT                0x0B
#define MB_FC_GT_CELOG                0x0C
#define MB_FC_WR_MCOILS               0x0F
#define MB_FC_WR_MREGS                0x10
                                      
#define MB_FC_REP_SID                 0x11
#define MB_FC_RD_FREC                 0x14
#define MB_FC_WR_FREC                 0x15
#define MB_FC_WR_REGM                 0x16
#define MB_FC_RW_MREGS                0x17
#define MB_FC_RD_FIFOQ                0x18
                                      
#define MB_FC_EIT                     0x2B
    #define MB_SFC_EIT_CANOPEN        0x0D
    #define MB_SFC_EIT_RDDEVID        0x0E
        
#define MB_FC_DIAGNOSTICS             0x08
    #define MB_SFC_DAIG_RETQDATA      0x0000
    #define MB_SFC_DAIG_RESTCOMM      0x0001
    #define MB_SFC_DAIG_RETDIAGR      0x0002
    #define MB_SFC_DAIG_CHASCDELIM    0x0003
    #define MB_SFC_DAIG_FORCELISTEN   0x0004
    #define MB_SFC_DAIG_CLRCNTS       0x000A
    #define MB_SFC_DAIG_RETBMCNT      0x000B
    #define MB_SFC_DAIG_RETBCECNT     0x000C
    #define MB_SFC_DAIG_RETBEECNT     0x000D
    #define MB_SFC_DAIG_RETSMCNT      0x000E
    #define MB_SFC_DAIG_RETSNRCNT     0x000F
    #define MB_SFC_DAIG_RETSNKCNT     0x0010
    #define MB_SFC_DAIG_RETSBUSYCNT   0x0011
    #define MB_SFC_DAIG_RETBCHOVCNT   0x0012
    #define MB_SFC_DAIG_CLRBCHOVCNT   0x0014
/**@}*/ 

/**
 * @name Modbus Function Code Handler Management and Dispatch
 * 
 */
/**@{*/ 
const modbus_fcode_handler_t* modbus_get_fcode_handler(uint8_t fcode);
/**@}*/ 

#endif