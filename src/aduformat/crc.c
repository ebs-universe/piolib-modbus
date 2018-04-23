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
 * @file aduformat/crc.c
 * @brief Implementation of the Modbus library.
 *
 */



#include "crc.h"


uint16_t modbus_calculate_crc(uint8_t * cmd, uint8_t len)
{
  uint16_t crc = 0xFFFF;
  uint8_t pos, i;
  
  for (pos = 0; pos < len; pos++) {
    crc ^= (uint16_t)cmd[pos];        // XOR byte into least sig. byte of crc
  
    for (i = 8; i != 0; i--) {        // Loop over each bit
      if ((crc & 0x0001) != 0) {      // If the LSB is set
        crc >>= 1;                    // Shift right and XOR 0xA001
        crc ^= 0xA001;
      }
      else                            // Else LSB is not set
        crc >>= 1;                    // Just shift right
    }
    
  }
  return crc;  
}

