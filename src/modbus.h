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
 * The bulk of this library corresponds roughly to the OSI Layer 7. 
 * 
 * The bulk of the OSI Layer 2 / 3 implementation of the protocol, the 
 * harware and link specification, is left to other libraries and utilized 
 * here from within the modbus state machine via the application-dependent 
 * external functions, which should be implemented in the application and 
 * be available at link time. 
 * 
 * Alternatively, the modbus state machine can be bypassed entirely and the
 * application must implement the state machine. It can use the layer 2/3 
 * helper functions to aid in handling the protocol. The modbus command 
 * handling functions should be called once the ADU is verified and the 
 * buffers are ready with the full PDU as described below. 
 * 
 * The nature of MODBUS presents a complication related to operating MODBUS 
 * over an interface such as CDC, where timing cannot be guaranteed. As a 
 * result, the underlying layer 2 implementation is insufficent to 
 * properly detect start and end of packets. 
 * 
 * This file therefore includes various helper functions that flesh out
 * the lower layers of the interface. These helper functions can be used 
 * to help handle packet reception. It should be assumed that these 
 * functions will be called from within an interrupt context.
 * 
 * Once the packet is fully recieved into the modbus_rxbuf, the primary 
 * modbus API function `modbus_process_command()` should be called. This 
 * function will then fill out the modbus_txbuf buffer, and hand it off 
 * to the underlying layer 3 implementation for a relatively simpler
 * transmission.
 * 
 * It should be noted that this MODBUS implementation would scale poorly to 
 * multiple devices, since each message sent over the bus will have to be
 * recieved and processed for layer 2/3 to determine where the next message
 * starts. 
 * 
 * Additionally, care must be taken while handling communications errors. 
 * MODBUS relies on the 3.5t and 1.5t timers to reset the interface after
 * a failed packet transmission. This implementation does not allow for 
 * those timers. Additional timeouts on both the slave and master may be 
 * necessary to resynchronize the interface. 
 * 
 * 
 * @see <http://www.modbus.org/docs/Modbus_Application_Protocol_V1_1b3.pdf>
 * @see modbus.c
 */


#ifndef MODBUS_H
#define MODBUS_H

#define MODBUS_ADU_MAXLEN               256
#define MODBUS_DEFAULT_DEVICE_ADDRESS   0x05

#include <stdint.h>

/**
 * @name Modbus Configuration Containers
 * 
 * Most of the containers defined here should be defined in the application
 * layer as per the requirements of the application. 
 */
/**@{*/ 

/** 
 *\brief UCDM Base Address for Modbus parameters. 
 */
extern const uint8_t ucdm_modbus_base_address;

#define UCDM_MODBUS_DEVICE_ADDRESS      ucdm_modbus_base_address

/**@}*/ 

/**
 * @name Modbus Internal Containers
 * 
 */
/**@{*/ 

extern uint8_t modbus_sm_state;
extern uint8_t modbus_sm_rxlen;

extern uint8_t modbus_rxbuf[MODBUS_ADU_MAXLEN];
extern uint8_t modbus_txbuf[MODBUS_ADU_MAXLEN];

/**@}*/ 

/**
 * @name Modbus Init and Maintenance Functions
 * 
 */
/**@{*/ 

void modbus_init(void);
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

/**@}*/ 

/**
 * @name Modbus Application Dependent External API Functions
 * 
 * These functions related to hardware or application-specific interfaces and 
 * must be provided by the application, probably within the subsystem layer. 
 * 
 * The function declarations are written to be transport and platform agnostic. 
 * An unfortunate side-effect of this is that the functions will have to be 
 * real ones and cannot simply be inlined. This will result in an unavoidable 
 * performance hit which is not presently quantified.
 * 
 * The modbus library is thus implemented here to first copy out as many bytes 
 * as it can into a modbus library buffer, within which the commmand may 
 * then be processed and handled. In applications requiring higher modbus 
 * command throughput, this intermediate buffer should be bypassed and the 
 * modbus commands handled directly against the bytebuf. The best approach to 
 * do this would be for the application to provide (and use) it's own modbus 
 * state machine implementation, using the sources for the one implemented 
 * here as a guide.
 * 
 * The implementation here does not use this approach to avoid the assumption
 * of the presence of bytebufs at the underlying hardware interface driver. 
 * This assumption would probably make the implementation incompatible with 
 * regular buffers or other interface types. This way, the protocol library 
 * is well separated from the hardware driver and we rely on the HAL and
 * subsystem layers to sort out how to perform specific, well defined tasks 
 * and just link against them.
 * 
 */
/**@{*/ 
extern void modbus_if_init(void);

extern uint8_t modbus_if_unhandled_rxb(void);
extern uint8_t modbus_if_getc(void);
extern uint8_t modbus_if_read(void *buffer, uint8_t len);

extern uint8_t modbus_if_available_wrb(void);
extern uint8_t modbus_if_putc(uint8_t byte);
extern uint8_t modbus_if_write(void *buffer, uint8_t len);
/**@}*/ 

/**
 * @name Modbus Lower Layer Helper Functions
 * 
 */
/**@{*/ 

/**
  * \brief MODBUS Layer 2/3 Helper function to determine command length
  * 
  * This function accepts a pointer to the start of the PDU thus far recieved, and 
  * returns the number of additional bytes that need to be read in. If the data thus far
  * is insufficient to determine full command length, then the number of bytes that are 
  * required to make that determination is returned. This function should be called 
  * repeatedly with increasing length of the command available until it returns 0, 
  * indicating that the command is completely recieved.
  * 
  * @param *cmd Pointer to the modbus command rx buffer with the partial command. This
  *             should point to the first byte of the ADU, ie, the address byte.
  * @param len Number of bytes already available.
  */
uint8_t modbus_get_res_clen(uint8_t * cmd, uint8_t len);

/**
  * \brief MODBUS Layer 2/3 Helper function to validate the recieved message
  * 
  * This function accepts a pointer to the start of the PDU and the length of 
  * the message. It confirms the CRC, confirms the address is relevant to this 
  * slave. It returns 1 for a valid message and 0 for an invalid one. This 
  * function also increments the appropriate counters.
  * 
  * If layer 2/3 gets a zero from here, it should discard the packet. If it 
  * recieves 1, it should then call modbus_process_command() so that the 
  * library can handle the message.
  * 
  * @param *cmd Pointer to the modbus command rx buffer with the full command. 
  *             This should point to the first byte of the ADU, ie, the address byte.
  * @param len Number of bytes already available.
  */
uint8_t modbus_validate_message(uint8_t * cmd, uint8_t len, uint16_t crc);

/**
  * \brief MODBUS Layer 2/3 Helper function to calculate the CRC
  * 
  * This function accepts a pointer to the start of the PDU and the length of 
  * the message. It returns a MODBUS compatible CRC value. 
  * 
  * This function is provided as an independent, portable CRC generator which 
  * layer 2/3 can use if it wants to. This fuction isn't likely to be fast, 
  * but it is small. Alternatively, layer 2/3 can implement it's own CRC 
  * generation function and use that instead.
  * 
  * @param *cmd Pointer to the modbus command rx buffer with the full command. 
  *             This should point to the first byte of the ADU, ie, the address byte.
  * @param len Number of bytes already available.
  */
uint16_t modbus_calculate_crc(uint8_t * cmd, uint8_t len);

/**@}*/ 

#endif