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


#include<bytebuf/bytebuf.h>

#include "diagnostics.h"

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

/**@}*/ 

/**
 * @name Event Log
 */
/**@{*/ 

uint16_t modbus_comm_event_cnt;
uint8_t modbus_comm_event_log_buf[64];
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
}

void modbus_clear_diagnostics(void){
    modbus_clear_eventlog();
    modbus_clear_counters();
}

void modbus_init_eventlog(void){
    bytebuf_vInit(modbus_comm_event_log_buf, 64, &modbus_comm_event_log);
    modbus_comm_event_cnt = 0;
    modbus_clear_eventlog();
}

void modbus_init_diagnostics(void){
    modbus_init_eventlog();
    modbus_clear_counters();
}
