/* 
   Copyright (c)
     (c) 2015-2018 Chintalagiri Shashank, Quazar Technologies Pvt. Ltd.
   
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
 * @file aduformat/crc.h
 * @brief Interface and usage of the Modbus library.
 *
 */


#ifndef MODBUS_CRC_H
#define MODBUS_CRC_H

#include <stdint.h>

/**
 * @name Modbus ADU Helper Functions
 * 
 */
/**@{*/ 

/**
  * \brief MODBUS Layer 2/3 Helper function to calculate the CRC
  * 
  * This function accepts a pointer to the start of the ADU and the length of 
  * the message. It returns a MODBUS compatible CRC value. 
  * * 
  * @param cmd Pointer to the modbus command rx buffer with the full command. 
  *             This should point to the first byte of the ADU, ie, the address byte.
  * @param len Number of bytes already available.
  */
uint16_t modbus_calculate_crc(uint8_t * cmd, uint8_t len);

/**@}*/ 

#endif
