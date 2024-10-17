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


#ifndef MODBUS_CONFIG_H
#define MODBUS_CONFIG_H

#include <application.h>

#define MODBUS_VERSION                      "modbus 0.1.0"

/**
* @name Modbus Interface Constants
*/
/**@{*/
#define MODBUS_ADUFORMAT_UART               1
/**@}*/

#ifndef MODBUS_LIBVERSION_DESCRIPTOR
#ifdef APP_ENABLE_LIBVERSION_DESCRIPTORS
    #define MODBUS_LIBVERSION_DESCRIPTOR    APP_ENABLE_LIBVERSION_DESCRIPTORS
#else
    #define MODBUS_LIBVERSION_DESCRIPTOR    0
#endif
#endif

#ifndef MODBUS_TRANSPORT_INTFNUM
    #define MODBUS_TRANSPORT_INTFNUM        0
#endif

#ifndef MODBUS_TRANSPORT_TYPE
    #define MODBUS_TRANSPORT_TYPE           0
#endif

#ifndef MODBUS_PLUGGABLE_TRANSPORTS    
    #define MODBUS_PLUGGABLE_TRANSPORTS     0
#endif

#if MODBUS_PLUGGABLE_TRANSPORTS
#ifndef MODBUS_ENABLE_TRANSPORT_UART
    #define MODBUS_ENABLE_TRANSPORT_UART    0
#endif
 
#ifndef MODBUS_TRANSPORT_UART_INTFNUM
    #define MODBUS_TRANSPORT_UART_INTFNUM   0
#endif

#ifndef MODBUS_ENABLE_TRANSPORT_USBCDC
    #define MODBUS_ENABLE_TRANSPORT_USBCDC  0
#endif

#ifndef MODBUS_TRANSPORT_USBCDC_INTFNUM
    #define MODBUS_TRANSPORT_USBCDC_INTFNUM 1
#endif
#endif

#ifndef MODBUS_RXCHUNKSIZE
    #define MODBUS_RXCHUNKSIZE              4
#endif

#ifndef MODBUS_TXCHUNKSIZE
    #define MODBUS_TXCHUNKSIZE              1
#endif

#ifndef BYTEBUF_TOKEN_MODBUS
    #define BYTEBUF_TOKEN_MODBUS            0x06
#endif


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

#ifndef MODBUS_FRAME_UNSUPPORTED
    #define MODBUS_FRAME_UNSUPPORTED        1
#endif

#ifndef MODBUS_EVENTLOG_LENGTH
    #define MODBUS_EVENTLOG_LENGTH          64
#endif

/**
 * @name Modbus Function Groups Support Inclusion
 */
/**@{*/
#ifndef MB_DIAGNOSTICS
    #define MB_DIAGNOSTICS                  1
#endif

#ifndef MB_SUPPORT_BITFUNCS            
    #define MB_SUPPORT_BITFUNCS             1
#endif

#ifndef MB_SUPPORT_REGFUNCS            
    #define MB_SUPPORT_REGFUNCS             1
#endif

#ifndef MB_SUPPORT_FILEFUNCS            
    #define MB_SUPPORT_FILEFUNCS            0
#endif

#ifndef MB_SUPPORT_CELOGFUNCS            
    #define MB_SUPPORT_CELOGFUNCS           MB_DIAGNOSTICS
#endif

#ifndef MB_SUPPORT_EITFUNCS
    #define MB_SUPPORT_EITFUNCS             1
#endif

/**@}*/

/**
 * @name Modbus Individual Function Codes Support Inclusion
 */
/**@{*/

#ifndef MB_SUPPORT_FC_RD_COILS
    #define MB_SUPPORT_FC_RD_COILS          MB_SUPPORT_BITFUNCS
#endif
#ifndef MB_SUPPORT_FC_RD_INPUTS
    #define MB_SUPPORT_FC_RD_INPUTS         MB_SUPPORT_BITFUNCS
#endif
#ifndef MB_SUPPORT_FC_WR_SCOIL
    #define MB_SUPPORT_FC_WR_SCOIL          MB_SUPPORT_BITFUNCS
#endif
#ifndef MB_SUPPORT_FC_WR_MCOILS
    #define MB_SUPPORT_FC_WR_MCOILS         MB_SUPPORT_BITFUNCS 
#endif


#ifndef MB_SUPPORT_FC_RD_HREG
    #define MB_SUPPORT_FC_RD_HREG           MB_SUPPORT_REGFUNCS
#endif 
#ifndef MB_SUPPORT_FC_RD_IREG
    #define MB_SUPPORT_FC_RD_IREG           MB_SUPPORT_REGFUNCS
#endif
#ifndef MB_SUPPORT_FC_WR_SREG
    #define MB_SUPPORT_FC_WR_SREG           MB_SUPPORT_REGFUNCS
#endif
#ifndef MB_SUPPORT_FC_WR_MREGS
    #define MB_SUPPORT_FC_WR_MREGS          MB_SUPPORT_REGFUNCS 
#endif
#ifndef MB_SUPPORT_FC_WR_REGM
    #define MB_SUPPORT_FC_WR_REGM           MB_SUPPORT_REGFUNCS
#endif
#ifndef MB_SUPPORT_FC_RW_MREGS
    #define MB_SUPPORT_FC_RW_MREGS          MB_SUPPORT_REGFUNCS 
#endif


#ifndef MB_SUPPORT_FC_RD_EXCST
    #define MB_SUPPORT_FC_RD_EXCST          1
#endif
#ifndef MB_SUPPORT_FC_REP_SID
    #define MB_SUPPORT_FC_REP_SID           0
#endif


#ifndef MB_SUPPORT_FC_GT_CECNT
    #define MB_SUPPORT_FC_GT_CECNT          MB_SUPPORT_CELOGFUNCS 
#endif
#ifndef MB_SUPPORT_FC_GT_CELOG
    #define MB_SUPPORT_FC_GT_CELOG          MB_SUPPORT_CELOGFUNCS  
#endif


#ifndef MB_SUPPORT_FC_RD_FREC
    #define MB_SUPPORT_FC_RD_FREC           MB_SUPPORT_FILEFUNCS 
#endif 
#ifndef MB_SUPPORT_FC_WR_FREC
    #define MB_SUPPORT_FC_WR_FREC           MB_SUPPORT_FILEFUNCS
#endif


#ifndef MB_SUPPORT_FC_RD_FIFOQ
    #define MB_SUPPORT_FC_RD_FIFOQ          0
#endif


#ifndef MB_SUPPORT_FC_EIT
    #define MB_SUPPORT_FC_EIT               MB_SUPPORT_EITFUNCS
#endif 
#ifndef MB_SUPPORT_SFC_EIT_CANOPEN
        #define MB_SUPPORT_SFC_EIT_CANOPEN  0
#endif
#ifndef MB_SUPPORT_SFC_EIT_RDDEVID
        #define MB_SUPPORT_SFC_EIT_RDDEVID  MB_SUPPORT_EITFUNCS
#endif
#ifndef MB_SUPPORT_FC_DIAGNOSTICS
    #define MB_SUPPORT_FC_DIAGNOSTICS       MB_DIAGNOSTICS
#endif

/**@}*/ 

#endif
