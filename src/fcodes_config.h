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
 * @file fcodes_config.h
 * @brief Configuration of Function Code Handler Inclusion.
 */


#ifndef MODBUS_FCODES_CONFIG_H
#define MODBUS_FCODES_CONFIG_H

/**
 * @name Modbus Function Codes Support Inclusion
 */
/**@{*/ 
#define MB_SUPPORT_FC_RD_COILS                  0
#define MB_SUPPORT_FC_RD_INPUTS                 0               
#define MB_SUPPORT_FC_RD_HREG                   1
#define MB_SUPPORT_FC_RD_IREG                   1
#define MB_SUPPORT_FC_WR_SCOIL                  1
#define MB_SUPPORT_FC_WR_SREG                   1
#define MB_SUPPORT_FC_RD_EXCST                  1
                                      
#define MB_SUPPORT_FC_GT_CECNT                  1
#define MB_SUPPORT_FC_GT_CELOG                  1
#define MB_SUPPORT_FC_WR_MCOILS                 0
#define MB_SUPPORT_FC_WR_MREGS                  1
                                      
#define MB_SUPPORT_FC_REP_SID                   1
#define MB_SUPPORT_FC_RD_FREC                   0
#define MB_SUPPORT_FC_WR_FREC                   0
#define MB_SUPPORT_FC_WR_REGM                   1 
#define MB_SUPPORT_FC_RW_MREGS                  1
#define MB_SUPPORT_FC_RD_FIFOQ                  0
                                      
#define MB_SUPPORT_FC_EIT                       0
    #define MB_SUPPORT_SFC_EIT_CANOPEN          1
    #define MB_SUPPORT_SFC_EIT_RDDEVID          1
        
#define MB_SUPPORT_FC_DIAGNOSTICS               1
    
/**@}*/ 

#endif