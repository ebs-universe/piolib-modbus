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
 * @file modbus.h
 * @brief Interface and usage of the Modbus library.
 *
 * This file is the Public API of the MODBUS library.
 * 
 * This file provides a library to handle the MODBUS application protocol. 
 * 
 * The bulk of the transport layer of the protocol is left to other libraries 
 * and utilized here from within the modbus state machine, via the 
 * application-dependent external functions. These functions should be 
 * implemented in the application and be available at link time. 
 * 
 * Alternatively, the modbus state machine can be bypassed entirely and the
 * application must implement the state machine. It can use the transport  
 * helper functions to aid in handling the protocol. The modbus command 
 * handling functions should be called once the ADU is verified and the 
 * buffers are ready with the full PDU as described below. 
 * 
 * The nature of MODBUS presents a complication related to operating MODBUS 
 * over an interface such as CDC, where timing cannot be guaranteed. As a 
 * result, the underlying transport implementation is insufficent to 
 * properly detect start and end of packets. 
 * 
 * This file includes various helper functions that flesh out the lower 
 * layers of the interface. These functions can be used to help handle packet 
 * reception. It should be assumed that these functions will be called from 
 * within an interrupt context.
 * 
 * Once the packet is fully recieved into the modbus_rxbuf, the primary 
 * modbus API function `modbus_process_command()` should be called. This 
 * function will then fill out the modbus_txbuf buffer, and hand it off 
 * to the underlying transport layer for a relatively simple transmission.
 * 
 * It should be noted that this MODBUS implementation would scale poorly to 
 * multiple devices, since each message sent over the bus will have to be
 * recieved and processed by the transport layers, including verification, 
 * to determine where the next message starts. 
 * 
 * Additionally, care must be taken while handling communications errors. 
 * MODBUS relies on the 3.5t and 1.5t timers to reset the interface after
 * a failed packet transmission. This implementation has a much simplified 
 * set of timers. Additional timeouts on both the slave and master may be 
 * necessary to resynchronize the interface. 
 * 
 * 
 * @see <http://www.modbus.org/docs/Modbus_Application_Protocol_V1_1b3.pdf>
 * @see modbus.c
 */


#ifndef MODBUS_H
#define MODBUS_H

#include <stdint.h>
#include <platform/transport.h>
#include "config.h"
#include "fcodes/common.h"

#define MODBUS_TIMEOUT_INTERNAL         0
#define MODBUS_TIMEOUT_CRON             1  // Not currently implemented
#define MODBUS_TIMEOUT_TYPE             MODBUS_TIMEOUT_INTERNAL

/**
 * @name Modbus State Machine State Variable States
 */
/**@{*/ 
#define MODBUS_ST_PREINIT       0
#define MODBUS_ST_IDLE          1
#define MODBUS_ST_RECV          2
#define MODBUS_ST_VERIFY        3
#define MODBUS_ST_PROCESS       4
#define MODBUS_ST_SEND          5
/**@}*/ 

/**
 * @name Modbus State Machine Silence States
 */
/**@{*/ 
#define MODBUS_OUT_NORMAL       0
#define MODBUS_OUT_SILENT       1
/**@}*/ 

/**
 * @name Modbus Current Transaction Types
 */
/**@{*/ 
#define MODBUS_CTT_UNICAST      0
#define MODBUS_CTT_BROADCAST    1
/**@}*/ 


/**
 * @name Modbus Internal Containers and Types
 * 
 */
/**@{*/ 

typedef struct MODBUS_ADUFORMAT_t
{
    const uint8_t prefix_n;
    const uint8_t postfix_n;
    void (*const pack)(void);
    uint8_t (*const validate)(void);
    void (*const write)(void);
}modbus_aduformat_t;

typedef struct MODBUS_SM_t
{
    const modbus_aduformat_t * aduformat;
#if MODBUS_PLUGGABLE_TRANSPORTS == 1
    const pluggable_transport_t * transport;
    uint8_t intfnum;
#endif 
    uint8_t state;
    uint8_t silent;
    uint8_t rxtxlen;
    uint8_t const* rxtxbuf;
    uint8_t * txp;
}modbus_sm_t;

typedef struct MODBUS_CTRANS_t
{
    uint8_t broadcast;
    uint8_t fcode;
    const modbus_fcode_handler_t * fcode_handler;
}modbus_ctrans_t;


extern uint16_t * modbus_address_p;
extern modbus_sm_t modbus_sm;
extern modbus_ctrans_t modbus_ctrans;
extern const modbus_aduformat_t modbus_aduformat;
extern uint8_t modbus_rxtxbuf[MODBUS_ADU_MAXLEN];

/**@}*/ 

/**
 * @name Modbus Init and Maintenance Functions
 * 
 */
/**@{*/ 

uint16_t modbus_init(uint16_t ucdm_next_address, uint16_t tmodbus_address);
void modbus_install_descriptor(void);
void modbus_set_address(uint16_t tmodbus_address);
void modbus_reset_sm(void);
void modbus_reset_all(void);

/**@}*/ 


/**
 * @name Modbus Primary API Functions
 * 
 */
/**@{*/ 

void modbus_state_machine(void);
uint8_t modbus_process_command(void);
extern uint16_t modbus_bus_char_overrun_cnt;

/**@}*/ 

/**
 * @name Modbus Lower Layer Helper Functions
 * 
 */
/**@{*/ 

/**
  * \brief MODBUS Layer 2/3 Helper function to calculate the CRC
  * 
  * This function accepts a pointer to the start of the ADU and the length of 
  * the message. It returns a MODBUS compatible CRC value. 
  * 
  * This function is provided as an independent, portable CRC generator which 
  * layer 2/3 can use if it wants to. This fuction isn't likely to be fast, 
  * but it is small. Alternatively, layer 2/3 can implement it's own CRC 
  * generation function and use that instead.
  * 
  * @param cmd Pointer to the modbus command rx buffer with the full command. 
  *             This should point to the first byte of the ADU, ie, the address byte.
  * @param len Number of bytes already available.
  */
uint16_t modbus_calculate_crc(uint8_t * cmd, uint8_t len);


/**@}*/ 

#endif
