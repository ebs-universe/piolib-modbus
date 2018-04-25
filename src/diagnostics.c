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
 * @file diagnostics.c
 * @brief Modbus Diagnostics Counters and Related
 *
 * See diagnostics.h for usage documentation. Implementation 
 * documentation should be added to this file as some point. 
 * 
 * @see diagnostics.h
 */


#include "config.h"
#include <ucdm/ucdm.h>
#include "diagnostics.h"
#include "modbus.h"


uint16_t *const modbus_diagnostic_register_p =  &ucdm_diagnostic_register;
uint8_t  *const modbus_exception_status_p =     &ucdm_exception_status;

#if MB_DIAGNOSTICS

/**
 * @name Diagnostic Counters
 */
/**@{*/ 

uint16_t modbus_bus_msg_cnt;
uint16_t modbus_bus_comm_err_cnt;
uint16_t modbus_bus_exception_cnt;
uint16_t modbus_bus_char_overrun_cnt;
uint16_t modbus_server_msg_cnt;
uint16_t modbus_server_noresp_cnt;
uint16_t modbus_server_nak_cnt;
uint16_t modbus_server_busy_cnt;
uint16_t modbus_comm_event_cnt;

/**@}*/ 

/**
 * @name Event Log
 */
/**@{*/ 

uint8_t modbus_comm_event_log_buf[MODBUS_EVENTLOG_LENGTH + 1];
bytebuf modbus_comm_event_log;

/**@}*/ 

void modbus_clear_eventlog(void){
    bytebuf_vFlush(&modbus_comm_event_log);
}

void modbus_clear_counters(void){
    modbus_bus_msg_cnt = 0;
    modbus_bus_comm_err_cnt = 0;
    modbus_bus_exception_cnt = 0;
    modbus_bus_char_overrun_cnt = 0;
    modbus_server_msg_cnt = 0;
    modbus_server_noresp_cnt = 0;
    modbus_server_nak_cnt = 0;
    modbus_server_busy_cnt = 0;
    modbus_comm_event_cnt = 0;
}

void modbus_clear_diagnostics(void){
    modbus_clear_eventlog();
    modbus_clear_counters();
    *modbus_diagnostic_register_p = 0x0000;
}

void modbus_init_eventlog(void){
    bytebuf_vInit(modbus_comm_event_log_buf, 
                  MODBUS_EVENTLOG_LENGTH, 
                  &modbus_comm_event_log);
    modbus_clear_eventlog();
}

void modbus_init_diagnostics(void){
    modbus_init_eventlog();
    modbus_clear_counters();
}

#else
void modbus_clear_eventlog(void){
    ;
}
void modbus_clear_counters(void){
    ;
}
void modbus_clear_diagnostics(void){
    ;
}
void modbus_init_eventlog(void){
    ;
}    
void modbus_init_diagnostics(void){
    ;
}
#endif

#if MB_SUPPORT_CELOGFUNCS
void modbus_append_event(uint8_t event){
    while(!bytebuf_cPushReqLock(&modbus_comm_event_log, 1, 0x01)){
        bytebuf_cPopByte(&modbus_comm_event_log);
    }
    bytebuf_cPushByte(&modbus_comm_event_log, event, 0x01);
}

void modbus_log_received(uint8_t event){
    event |= (1<<7);
    if (modbus_sm.silent){
        event |= (1<<5);
    }
    if (modbus_bus_char_overrun_cnt){
        // TODO Does this make sense per event?
        event |= (1<<4);
    }
    modbus_append_event(event);
}

void modbus_log_sent(uint8_t event){
    event |= (1<<6);
    if (modbus_sm.silent){
        event |= (1<<5);
    }
    modbus_append_event(event);
}

#else
void modbus_append_event(uint8_t __attribute__((unused)) event){
    ;
}
void modbus_log_received(uint8_t __attribute__((unused)) event){
    ;
}
void modbus_log_sent(uint8_t __attribute__((unused)) event){
    ;
}
#endif
