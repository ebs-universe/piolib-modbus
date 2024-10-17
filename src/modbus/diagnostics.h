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
 * @see diagnostics.c
 */

#ifndef MODBUS_DIAGNOSTICS_H
#define MODBUS_DIAGNOSTICS_H

#include "config.h"
#include <ds/bytebuf.h>


extern uint16_t *const modbus_diagnostic_register_p;
extern uint8_t  *const modbus_exception_status_p;

/**
 * @name Diagnostic Counters
 */
/**@{*/ 

/**
 * \brief Bus message counter 
 * 
 * The number of messages that the slave has detected on the communications 
 * system. 
 * 
 * Incremented by the aduformat validate function. This is implemented for 
 * the included UART ADU format, but must be specifically implemented for 
 * each ADU format added to the library or provided by the application.
 * 
 * @see modbus_uart_adu_validate()
 */
extern uint16_t modbus_bus_msg_cnt;

/**
 * \brief Bus communication errors counter 
 * 
 * The quantity of CRC errors encountered by the slave.
 *  
 * Incremented by the aduformat validate function. This is implemented for 
 * the included UART ADU format, but must be specifically implemented for 
 * each ADU format added to the library or provided by the application.
 * 
 * @see modbus_uart_adu_validate()
 */
extern uint16_t modbus_bus_comm_err_cnt;

/**
 * \brief Bus exception counter 
 * 
 * The quantity of Modbus exception responses returned by the slave.
 * 
 * Incremented by the exception response builder of fcodes/common.c.
 * 
 * @see modbus_build_exc_response()
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
 * This counter is left to the application to imcrement, usually from within 
 * its interface to the modbus library. 
 * 
 * @see interface.h
 */
extern uint16_t modbus_bus_char_overrun_cnt;

/**
 * \brief Server message counter
 * 
 * The quantity of messages addressed to the remote device, or broadcast, that 
 * the remote device has processed.
 * 
 * Incremented by the modbus state machine through its command processor.
 * 
 * @see modbus_process_command()
 */
extern uint16_t modbus_server_msg_cnt;

/**
 * \brief Server no response counter
 * 
 * The quantity of messages addressed to the remote device for which it has
 * returned no response (neither a normal response nor an exception response).
 * 
 * Incremented directly by the modbus state machine.
 * 
 * @see modbus_state_machine()
 */
extern uint16_t modbus_server_noresp_cnt;

/**
 * \brief Server NAK counter
 * 
 * The quantity of messages addressed to the remote device for which it returned 
 * a Negative Acknowledge (NAK) exception response.
 * 
 * Incremented by the exception response builder of fcodes/common.c.
 * 
 * @see modbus_build_exc_response()
 */
extern uint16_t modbus_server_nak_cnt;


/**
 * \brief Server Busy counter
 * 
 * The quantity of messages addressed to the remote device for which it returned a
 * Server Device Busy exception response.
 * 
 * Incremented by the exception response builder of fcodes/common.c.
 * 
 * @see modbus_build_exc_response()
 */
extern uint16_t modbus_server_busy_cnt;

/**
 * \brief The controller’s event counter 
 * 
 * This is incremented once for each successful message completion. It is not 
 * incremented for exception responses, poll commands, or fetch event counter 
 * commands. 
 * 
 * This counter is not actually implemented.
 */
extern uint16_t modbus_comm_event_cnt;

/**@}*/ 

/**
 * @name Event Log
 * 
 * The actual event log is created across the sources : 
 * 
 * Recieve Event : 
 * 1 Communication Error    modbus_uart_adu_validate
 * 4 Character Overrun      modbus_log_received
 * 5 In Listen Only Mode    modbus_log_received
 * 6 Broadcast Recieved     modbus_state_machine
 * 7 1                      modbus_log_received
 * 
 * log_recieved is called from :
 *  - modbus_uart_adu_validate
 *  - modbus_state_machine
 *  - modbus_process_command
 * 
 * Send Event :
 * 0 Read Exception Sent (EC 1-3)               modbus_build_exc_response
 * 1 Server Abort Exception Sent (EC 4)         modbus_build_exc_response
 * 2 Server Busy Exception Sent (EC 5-6)        modbus_build_exc_response
 * 3 Server Program NAK Exception Sent (EC 7)   modbus_build_exc_response
 * 4 Write Timeout Error Occurred               no write timeout implemented
 * 5 Currently in Listen Only Mode              modbus_log_sent
 * 6 1                                          modbus_log_sent
 * 7 0                                          modbus_log_sent
 * 
 * log_sent is called from :
 *  - modbus_build_exc_response
 *  - modbus_process_command
 */
/**@{*/ 

extern uint8_t modbus_comm_event_log_buf[MODBUS_EVENTLOG_LENGTH + 1];
extern bytebuf modbus_comm_event_log;

/**@}*/ 

void modbus_clear_eventlog(void);
void modbus_clear_counters(void);
void modbus_clear_diagnostics(void);

void modbus_init_eventlog(void);
void modbus_init_diagnostics(void);

void modbus_append_event(uint8_t event);
void modbus_log_received(uint8_t event);
void modbus_log_sent(uint8_t event);

#endif
