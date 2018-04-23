/* 
   Copyright (c)
     (c) 2018 Chintalagiri Shashank, Quazar Technologies Pvt. Ltd.
   
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
 * @file aduformat/crc-msp430-elf.S
 * @brief Implementation of the Modbus library.
 *
 * Implementation based loosely on _crc_algs.s43 of TI Application Report 
 * slaa221 "CRC Implementation with MSP430"
 *
 * TODO : The Implementation there stores #8 into r_bitcnt. This requires an 
 *        addtional push and pop, in addition to the extra mov. Should check 
 *        whether #8 is really that much more expensive. Same for poly.
 */

 .file  "crc-msp430-elf.S"
 
#include <msp430.h>
#include <platform/asm.h>

#define r_j         R11
#define r_pmsg      R12
#define r_len       R13
#define r_crc       R14
#define r_msg       R15
#define r_poly      #-24575

    .section    .text.modbus_calculate_crc,"ax",@progbits
    .balign 2
    .global modbus_calculate_crc
    .type   modbus_calculate_crc, @function
modbus_calculate_crc:
/*  start of function */
/*  framesize:      0 */
/*  start of prologue */
/*  end of prologue */
    MOV.W       #-1,        r_crc
    CLR.W       r_msg
    CLR.W       r_j
_outer_loop:
    MOV.B       @r_pmsg+,   r_msg
    XOR.W       r_msg,      r_crc
    MOV.B       #8,         r_j
_inner_loop:
    CLRC
    RRC.W       r_crc
    JNC         _continue
    XOR.W       r_poly,     r_crc
_continue:
    DEC.B       r_j
    JNZ         _inner_loop
    DEC.B       r_len
    JNZ         _outer_loop
    MOV.W       r_crc,  R12
/*  start of epilogue */
    RET
/*  end of epilogue */
    .size   modbus_calculate_crc, .-modbus_calculate_crc
/*  end of function */
