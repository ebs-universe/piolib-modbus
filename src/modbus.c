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


#include <ucdm/ucdm.h>
#include <ucdm/descriptor.h>
#include <time/time.h>

#include "modbus.h"
#include "dispatch.h"
#include "diagnostics.h"
#include "interface.h"

#include "aduformat/uart.h"
#include "fcodes/crlen.h"

#if MODBUS_PLUGGABLE_TRANSPORTS == 1
#include "hotplug.h"
#endif

modbus_ctrans_t modbus_ctrans;
uint8_t modbus_rxtxbuf[MODBUS_ADU_MAXLEN];

#if MODBUS_USE_TIMEOUTS
    tm_system_t modbus_next_timeout;
    void _mbtimeout_change_handler(tm_sdelta_t * offset);
    tm_epochchange_handler_t _mbtimeout_epochchange_handler = {NULL, 3, &_mbtimeout_change_handler};
#endif

modbus_sm_t modbus_sm = {
    #if MODBUS_ADU_FORMAT == MODBUS_ADUFORMAT_UART
        &modbus_aduformat_uart,
    #endif
    #if MODBUS_PLUGGABLE_TRANSPORTS == 1
        NULL, 
        0,
    #endif 
        MODBUS_ST_PREINIT,
        MODBUS_OUT_NORMAL,
        0,
        &modbus_rxtxbuf[0],
        &modbus_rxtxbuf[0],
};

uint16_t * modbus_address_p;

/** @brief Modbus Library Version Descriptor */
static descriptor_custom_t modbus_descriptor = {NULL, 
    DESCRIPTOR_TAG_LIBVERSION, sizeof(MODBUS_VERSION), 
    DESCRIPTOR_ACCTYPE_PTR, {MODBUS_VERSION}};

void modbus_install_descriptor(void)
{
    descriptor_install(&modbus_descriptor);
}

void modbus_reset_sm(void){
    modbus_sm.rxtxlen = 0;
    modbus_sm.state = MODBUS_ST_IDLE;
    modbus_sm.txp = &modbus_rxtxbuf[0];
}

void modbus_reset_all(void){
    modbus_clear_diagnostics();
    modbus_reset_sm();
}

void modbus_set_address(uint16_t tmodbus_address){
    *modbus_address_p = tmodbus_address;
}

static inline uint16_t _modbus_init_interface(uint16_t ucdm_next_address, 
                                              uint16_t tmodbus_address);

static inline uint16_t _modbus_init_interface(uint16_t ucdm_next_address, 
                                              uint16_t tmodbus_address){
    modbus_if_init();
    modbus_address_p = &(ucdm_register[ucdm_next_address].data);
    modbus_set_address(tmodbus_address);
    return (ucdm_next_address + 1);
}

uint16_t modbus_init(uint16_t ucdm_next_address, uint16_t tmodbus_address){
    #if MODBUS_PLUGGABLE_TRANSPORTS == 1 
    modbus_init_ptransports(ucdm_next_address + 1);
    #endif
    _modbus_init_interface(ucdm_next_address, tmodbus_address);
    modbus_init_diagnostics();
    modbus_reset_all();
    #if MODBUS_USE_TIMEOUTS
    tm_register_epoch_change_handler(&_mbtimeout_epochchange_handler);
    #endif
    return ucdm_next_address + 2;
}

#if MODBUS_USE_TIMEOUTS
static inline void _mbtimeout_set(uint8_t seconds){
    tm_current_time(&modbus_next_timeout);
    modbus_next_timeout.seconds += seconds;
}

static inline void _mbtimeout_clear(void){
    tm_clear_stime(&modbus_next_timeout);
}

static inline uint8_t _mbtimeout_check(void){
    if (tm_cmp_stime(&tm_current, &modbus_next_timeout) == 1){
        return 1;
    }
    else{
        return 0;
    }
}

void _mbtimeout_change_handler(tm_sdelta_t * offset){
    tm_apply_sdelta(&modbus_next_timeout, offset);
}
#else 
static inline void _mbtimeout_set(uint8_t seconds){
    ;    
}
static inline void _mbtimeout_clear(void){
    ;
}
static inline uint8_t _mbtimeout_check(void){
    return 0;
}
#endif


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
    uint8_t uvar8;
    #if MODBUS_PLUGGABLE_TRANSPORTS
    modbus_hotplug_check();
    #endif
    switch(modbus_sm.state){
        case MODBUS_ST_PREINIT:
            break;
        case MODBUS_ST_IDLE:
            tvar8 = modbus_sm.aduformat->prefix_n + 1;
            if (modbus_if_unhandled_rxb() >= tvar8){
                _mbtimeout_set(1);
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
                uvar8 = modbus_if_unhandled_rxb();
                if (uvar8 < tvar8){
                    if (uvar8 >= MODBUS_RXCHUNKSIZE){
                        tvar8 = uvar8;
                    }
                    else{
                        tvar8 = 0;
                    }
                }
                if (tvar8){
                    _mbtimeout_set(2);
                    modbus_if_read(&(modbus_rxtxbuf[modbus_sm.rxtxlen]), tvar8);
                    modbus_sm.rxtxlen += tvar8;
                }
                else{
                    if (_mbtimeout_check()) modbus_reset_sm();
                }
            }
            else{
                _mbtimeout_clear();
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
            if (modbus_sm.rxtxlen){
                modbus_sm.aduformat->write();
            }
            else{
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
