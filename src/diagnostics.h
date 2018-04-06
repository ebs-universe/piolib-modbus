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
 * @file diagnostics.h
 * @brief Modbus Diagnostics Counters and Related
 *
 * See diagnostics.c for implementation documentation.
 * 
 * @see diagnostics.c
 */

#ifndef MODBUS_DIAGNOSTICS_H
#define MODBUS_DIAGNOSTICS_H

#include<bytebuf/bytebuf.h>

/**
 * @name Diagnostic Counters
 */
/**@{*/ 

/**
 * \brief Bus message counter 
 * 
 * The quantity of messages that the slave has detected on the communications 
 * system. 
 * 
 * Incremented via helper function by Layer 3.
 * 
 * @see modbus_validate_message()
 */
extern uint16_t modbus_bus_msg_cnt;

/**
 * \brief Bus communication errors counter 
 * 
 * The quantity of CRC errors encountered by the slave.
 *  
 * Incremented via helper function by Layer 3.
 * 
 * @see modbus_validate_message()
 */
extern uint16_t modbus_bus_comm_err_cnt;

/**
 * \brief Bus exception counter 
 * 
 * The quantity of Modbus exception responses returned by the slave.
 */
extern uint16_t modbus_bus_exception_cnt;

/**
 * \brief Bus character overrun counter 
 * 
 * The quantity of messages addressed to the remote device that it could not 
 * handle due to a character overrun condition. A character overrun is caused 
 * by data characters arriving at the port faster than they can be stored, or 
 * by the loss of a character due to a hardware malfunction.
 * 
 * Incremented via helper function by Layer 2 / 3.
 */
extern uint16_t modbus_bus_char_overrun_cnt;

/**
 * \brief Server message counter
 * 
 * The quantity of messages addressed to the remote device, or broadcast, that 
 * the remote device has processed.
 * 
 * @see modbus_process_command()
 */
extern uint16_t modbus_server_msg_cnt;

/**
 * \brief Server no response counter
 * 
 * The quantity of messages addressed to the remote device for which it has
 * returned no response (neither a normal response nor an exception response).
 */
extern uint16_t modbus_server_noresp_cnt;

/**
 * \brief Server NAK counter
 * 
 * The quantity of messages addressed to the remote device for which it returned 
 * a Negative Acknowledge (NAK) exception response.
 */
extern uint16_t modbus_server_nak_cnt;


/**
 * \brief Server Busy counter
 * 
 * The quantity of messages addressed to the remote device for which it returned a
 * Server Device Busy exception response.
 */

extern uint16_t modbus_server_busy_cnt;

/**@}*/ 

/**
 * @name Event Log
 */
/**@{*/ 

/**
 * \brief The controller’s event counter 
 * 
 * This is incremented once for each successful message completion. It is not 
 * incremented for exception responses, poll commands, or fetch event counter 
 * commands. 
 */
extern uint16_t modbus_comm_event_cnt;
extern uint8_t modbus_comm_event_log_buf[64];
extern bytebuf modbus_comm_event_log;

/**@}*/ 

void modbus_clear_eventlog(void);
void modbus_clear_counters(void);
void modbus_clear_diagnostics(void);

void modbus_init_eventlog(void);
void modbus_init_diagnostics(void);


#endif
