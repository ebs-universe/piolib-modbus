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
 * @file modbus.c
 * @brief Implementation of the Modbus library.
 *
 * See modbus.h for usage documentation. Implementation 
 * documentation should be added to this file as some point. 
 * 
 * @see modbus.h
 */

#include <bytebuf/bytebuf.h>
#include <ucdm/ucdm.h>

#include "modbus.h"
#include "fcodes.h"
#include "diagnostics.h"

/**
 * @name Modbus State Machine State Variable States
 */
/**@{*/ 
#define MODBUS_ST_IDLE          0
#define MODBUS_ST_RECV          1
#define MODBUS_ST_VERIFY        2
#define MODBUS_ST_PROCESS       3
#define MODBUS_ST_SEND          4
/**@}*/ 

/**
 * \brief Modbus State Machine State Variable
 * 
 * Current state of the Modbus Interface State Machine.
 * Can be one of the following : 
 *  - MODBUS_ST_IDLE    0
 *  - MODBUS_ST_RECV    1
 *  - MODBUS_ST_VERIFY  2
 *  - MODBUS_ST_PROCESS 3
 *  - MODBUS_ST_SEND    4
 */
uint8_t modbus_sm_state;
uint8_t modbus_sm_rxlen;

uint8_t modbus_rxbuf[MODBUS_ADU_MAXLEN];
uint8_t modbus_txbuf[MODBUS_ADU_MAXLEN];



void modbus_reset_sm(void){
    modbus_sm_state = MODBUS_ST_IDLE;
    modbus_sm_rxlen = 0;
}

void modbus_reset_all(void){
    modbus_clear_diagnostics();
    modbus_reset_sm();
}

static inline void _modbus_init_interface(void);

static inline void _modbus_init_interface(void){
    modbus_if_init();
    ucdm_register[UCDM_MODBUS_DEVICE_ADDRESS] = MODBUS_DEFAULT_DEVICE_ADDRESS;
}

void modbus_init(void){
    _modbus_init_interface();
    modbus_init_diagnostics();
    modbus_reset_all();
}


// MODBUS State Machine Implementation

void modbus_state_machine(void){
    // This function can be moved to the application layer and reimplemented there
    // if it is desireable to better integrate the hardware driver layers with the 
    // modbus state machine. If this is to be done, copy out this function into the 
    // application layer, rename it, make the necessary changes, and call that new 
    // functioninstead of this one. Most variables and containers used here should 
    // be usable as is from the application layer, as long as only one of the state 
    // machines is used consistently within the application.
    if (modbus_sm_state == MODBUS_ST_IDLE){
        ;
    }
}

uint8_t modbus_process_command(void){
//     if (failure){
//         correct_cnt++;
//         return 0;
//     }
    modbus_server_msg_cnt ++;
    return 1;
}

uint8_t modbus_get_res_clen(uint8_t * cmd, uint8_t len){
    return 0;
}

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

uint8_t modbus_validate_message(uint8_t * cmd, uint8_t len, uint16_t crc){
    //Get Address included in message
    uint8_t apu_addr = *cmd;
    
    //Get CRC included in message
    uint16_t apu_crc = (uint16_t)(*(cmd + len - 2)) + ((uint16_t)(*(cmd + len - 1))<<8);
    
    if (apu_crc != crc){
        modbus_bus_comm_err_cnt ++;
        return 0;
    }
    
    modbus_bus_msg_cnt ++;
    
    if (apu_addr == ucdm_register[UCDM_MODBUS_DEVICE_ADDRESS]){
        return 1;
    }
    
    return 0;
}
