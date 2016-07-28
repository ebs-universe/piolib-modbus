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
 * @file aduformat/uart.h
 * @brief Interface and usage of the Modbus library. 
 * 
 * @see <http://www.modbus.org/docs/Modbus_over_serial_line_V1.pdf>
 * @see aduformat/uart.c
 */


#ifndef MODBUS_ADUFORMAT_UART_H
#define MODBUS_ADUFORMAT_UART_H

#include <stdint.h>
#include "../modbus.h"

extern const modbus_aduformat_t modbus_aduformat_uart;
/**@}*/ 


/**
  * \brief MODBUS Layer 2/3 Helper function to validate the recieved message
  * 
  * This function validates the message currently in the modbus_rxbuf. It 
  * verifies the CRC, confirms the address is relevant to this slave. 
  * It returns 1 for a valid message and 0 for an invalid one. This 
  * function also increments the appropriate counters. 
  * 
  * The command length is not validated here, and should be previously
  * verified using the crlen handlers.
  * 
  * If layer 2/3 gets a zero from here, it should discard the packet. If it 
  * recieves 1, it should then call modbus_process_command() so that the 
  * library can handle the message.
  */
uint8_t modbus_uart_adu_validate(void);
void modbus_uart_adu_pack(void);
void modbus_uart_adu_write(void);

/**@}*/ 

#endif