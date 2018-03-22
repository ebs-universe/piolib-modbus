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
 * @file config.h
 * @brief Configuration of Function Code Handler Inclusion.
 */


#define MODBUS_ADUFORMAT_UART               0x01

#ifndef MODBUS_ADU_FORMAT
    #define MODBUS_ADU_FORMAT               MODBUS_ADUFORMAT_UART
#endif
#ifndef MODBUS_ADU_MAXLEN
    #define MODBUS_ADU_MAXLEN               256
#endif
#ifndef MODBUS_DEFAULT_DEVICE_ADDRESS
    #define MODBUS_DEFAULT_DEVICE_ADDRESS   0x05
#endif
#ifndef MODBUS_USE_TIMEOUTS
    #define MODBUS_USE_TIMEOUTS             1
#endif


#ifndef MODBUS_FCODES_CONFIG_H
#define MODBUS_FCODES_CONFIG_H

/**
 * @name Modbus Function Codes Support Inclusion
 */
/**@{*/ 
#ifndef MB_SUPPORT_FC_RD_COILS
    #define MB_SUPPORT_FC_RD_COILS                  0
#endif
#ifndef MB_SUPPORT_FC_RD_INPUTS
    #define MB_SUPPORT_FC_RD_INPUTS                 0
#endif
#ifndef MB_SUPPORT_FC_RD_HREG
    #define MB_SUPPORT_FC_RD_HREG                   1
#endif
#ifndef MB_SUPPORT_FC_RD_IREG
    #define MB_SUPPORT_FC_RD_IREG                   1
#endif
#ifndef MB_SUPPORT_FC_WR_SCOIL
    #define MB_SUPPORT_FC_WR_SCOIL                  1
#endif
#ifndef MB_SUPPORT_FC_WR_SREG
    #define MB_SUPPORT_FC_WR_SREG                   1
#endif
#ifndef MB_SUPPORT_FC_RD_EXCST
    #define MB_SUPPORT_FC_RD_EXCST                  1
#endif
#ifndef MB_SUPPORT_FC_GT_CECNT
    #define MB_SUPPORT_FC_GT_CECNT                  1
#endif
#ifndef MB_SUPPORT_FC_GT_CELOG
    #define MB_SUPPORT_FC_GT_CELOG                  1
#endif
#ifndef MB_SUPPORT_FC_WR_MCOILS
    #define MB_SUPPORT_FC_WR_MCOILS                 0
#endif
#ifndef MB_SUPPORT_FC_WR_MREGS
    #define MB_SUPPORT_FC_WR_MREGS                  1
#endif
#ifndef MB_SUPPORT_FC_REP_SID
    #define MB_SUPPORT_FC_REP_SID                   0
#endif
#ifndef MB_SUPPORT_FC_RD_FREC
    #define MB_SUPPORT_FC_RD_FREC                   0
#endif
#ifndef MB_SUPPORT_FC_WR_FREC
    #define MB_SUPPORT_FC_WR_FREC                   0
#endif
#ifndef MB_SUPPORT_FC_WR_REGM
    #define MB_SUPPORT_FC_WR_REGM                   1 
#endif
#ifndef MB_SUPPORT_FC_RW_MREGS
    #define MB_SUPPORT_FC_RW_MREGS                  1
#endif
#ifndef MB_SUPPORT_FC_RD_FIFOQ
    #define MB_SUPPORT_FC_RD_FIFOQ                  0
#endif

#ifndef MB_SUPPORT_FC_EIT
    #define MB_SUPPORT_FC_EIT                       1
#endif
#if MB_SUPPORT_FC_EIT == 1
    #ifndef MB_SUPPORT_SFC_EIT_CANOPEN
        #define MB_SUPPORT_SFC_EIT_CANOPEN          0
    #endif
    #ifndef MB_SUPPORT_SFC_EIT_RDDEVID
        #define MB_SUPPORT_SFC_EIT_RDDEVID          1
    #endif
#endif

#ifndef MB_SUPPORT_FC_DIAGNOSTICS
    #define MB_SUPPORT_FC_DIAGNOSTICS               1
#endif    
/**@}*/ 

#endif
