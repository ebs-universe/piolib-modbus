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

modbus_ctrans_t modbus_ctrans;

const modbus_aduformat_t modbus_aduformat = {
    1, 
    2,
    &modbus_uart_adu_pack,
    &modbus_uart_adu_validate,
    &modbus_uart_adu_write,
};

uint8_t modbus_rxtxbuf[MODBUS_ADU_MAXLEN];

modbus_sm_t modbus_sm = {
    &modbus_aduformat,
    MODBUS_ST_PREINIT,
    MODBUS_OUT_NORMAL,
    0,
    &modbus_rxtxbuf[0],
};

void modbus_reset_sm(void){
    modbus_sm.rxtxlen = 0;
    modbus_sm.state = MODBUS_ST_IDLE;
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
    // function instead of this one. Most variables and containers used here should 
    // be usable as is from the application layer, as long as only one of the state 
    // machines is used consistently within the application.
    uint8_t tvar8;
    switch(modbus_sm.state){
        case MODBUS_ST_PREINIT:
            break;
        case MODBUS_ST_IDLE:
            tvar8 = modbus_sm.aduformat->prefix_n + 1;
            if (modbus_if_unhandled_rxb() >= tvar8){
                modbus_if_read(&(modbus_rxtxbuf[0]), tvar8);
                modbus_sm.rxtxlen = tvar8;
                modbus_ctrans.fcode = modbus_rxtxbuf[tvar8-1];
                modbus_ctrans.fcode_handler = modbus_get_fcode_handler(modbus_ctrans.fcode);
                modbus_sm.state = MODBUS_ST_RECV;
            };
            break;
        case MODBUS_ST_RECV:
            tvar8 = modbus_crlen();
            if (tvar8){
                if (modbus_if_unhandled_rxb() >= tvar8){
                    modbus_if_read(&(modbus_rxtxbuf[modbus_sm.rxtxlen]), tvar8);
                    modbus_sm.rxtxlen += tvar8;
                }
            }
            else{
                modbus_sm.state = MODBUS_ST_VERIFY;
            }
            break;
        case MODBUS_ST_VERIFY:
            if (modbus_sm.aduformat->validate()){
                modbus_sm.state = MODBUS_ST_PROCESS;
            }
            else{
                // CRC Failed / Command not for us.
                modbus_reset_sm();
            }
            break;
        case MODBUS_ST_PROCESS:
            modbus_process_command();
            if (modbus_ctrans.broadcast || !(modbus_sm.rxtxlen)){
                modbus_server_noresp_cnt ++;
                modbus_reset_sm();
            }
            else{
                modbus_sm.state = MODBUS_ST_SEND;
            }
            break;
        case MODBUS_ST_SEND:
            if (modbus_sm.aduformat->write()){
                modbus_reset_sm();
            }
            break;
    }
    
}

uint8_t modbus_process_command(void){
    if (modbus_sm.silent && (modbus_ctrans.fcode != MB_FC_DIAGNOSTICS)){
        modbus_sm.rxtxlen = 0;
        return 0;
    }
    modbus_ctrans.fcode_handler->handler();
    modbus_server_msg_cnt ++;
    return 1;
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

uint8_t modbus_uart_adu_validate(void){
    //Get Address included in message
    uint8_t apu_addr = modbus_rxtxbuf[0];
    //Get CRC included in message
    uint16_t apu_crc = (modbus_rxtxbuf[modbus_sm.rxtxlen-2]) | ((uint16_t)(modbus_rxtxbuf[modbus_sm.rxtxlen-1])<<8);
    // Calculate CRC of message
    uint16_t crc = modbus_calculate_crc(&(modbus_rxtxbuf[0]), (modbus_sm.rxtxlen-2));
    
    if (apu_crc != crc){
        modbus_bus_comm_err_cnt ++;
        return 0;
    }
    
    modbus_bus_msg_cnt ++;
    
    if (!apu_addr){
        modbus_ctrans.broadcast = MODBUS_CTT_BROADCAST;
    }
    else if(apu_addr == ucdm_register[UCDM_MODBUS_DEVICE_ADDRESS]){
        modbus_ctrans.broadcast = MODBUS_CTT_UNICAST;
    }
    else{
        return 0;
    }
    
    modbus_server_msg_cnt ++;
    return 1;
}

void modbus_uart_adu_pack(void){
    uint16_t crc;
    crc = modbus_calculate_crc(&modbus_rxtxbuf[0], 
                               modbus_sm.rxtxlen);
    modbus_rxtxbuf[modbus_sm.rxtxlen] = (uint8_t)crc;
    modbus_rxtxbuf[modbus_sm.rxtxlen+1] = (uint8_t)(crc >> 8);
    modbus_sm.rxtxlen += 2;
}

uint8_t modbus_uart_adu_write(void){
    if (modbus_if_reqlock(modbus_sm.rxtxlen)){
        modbus_if_write(&(modbus_rxtxbuf[0]), modbus_sm.rxtxlen);
        return 1;
    }
    return 0;
}
