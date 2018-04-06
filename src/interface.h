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
 * @file interface.h
 * @brief Interface and usage of the Modbus library.
 *
 */


#ifndef MODBUS_INTERFACE_H
#define MODBUS_INTERFACE_H

/**
* @name Modbus Interface Constants
*/
/**@{*/ 
#define MODBUS_UART               1
#define MODBUS_USBCDC             2
/**@}*/
    
#include "config.h"

#ifndef APP_MODBUSIF_TYPE

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
void modbus_if_init(void);

uint8_t modbus_if_unhandled_rxb(void);
uint8_t modbus_if_getc(void);
uint8_t modbus_if_read(void *buffer, uint8_t len);

uint8_t modbus_if_reqlock(uint8_t len);
uint8_t modbus_if_rellock(void);
uint8_t modbus_if_putc(uint8_t byte);
uint8_t modbus_if_write(void *buffer, uint8_t len);
void modbus_if_flush(void);
/**@}*/ 

#else

#if APP_MODBUSIF_TYPE == MODBUS_UART

#include "bsp/hal/uc/uart.h"

static inline void modbus_if_init(void){
    uart_if[APP_MODBUSIF_INTFNUM]->state->overrun_counter = &modbus_bus_char_overrun_cnt;
    uart_init(APP_MODBUSIF_INTFNUM);
}
static inline uint8_t modbus_if_unhandled_rxb(void){
    return uart_population_rxb(APP_MODBUSIF_INTFNUM);
}
static inline uint8_t modbus_if_getc(void){
    return uart_getc(APP_MODBUSIF_INTFNUM);
}
static inline uint8_t modbus_if_read(void *buffer, uint8_t len){
    return uart_read(APP_MODBUSIF_INTFNUM, buffer, len);
}
static inline uint8_t modbus_if_reqlock(uint8_t len){
    return uart_reqlock(APP_MODBUSIF_INTFNUM, len, BYTEBUF_TOKEN_MODBUS);
}
static inline uint8_t modbus_if_rellock(void){
    return uart_rellock(APP_MODBUSIF_INTFNUM, BYTEBUF_TOKEN_MODBUS);
}
static inline uint8_t modbus_if_putc(uint8_t byte){
    return uart_putc(APP_MODBUSIF_INTFNUM, byte, BYTEBUF_TOKEN_MODBUS, 0x00);
}
static inline uint8_t modbus_if_write(void *buffer, uint8_t len){
    return uart_write(APP_MODBUSIF_INTFNUM, buffer, len, BYTEBUF_TOKEN_MODBUS);
}
static inline void modbus_if_flush(void){
    uart_send_flush(APP_MODBUSIF_INTFNUM);
    return;
}
#elif APP_MODBUSIF_TYPE == MODBUS_USBCDC

#include "bsp/hal/uc/usbcdc.h"

static inline void modbus_if_init(void){
    return;
}
static inline uint8_t modbus_if_unhandled_rxb(void){
    return usbcdc_population_rxb(APP_MODBUSIF_INTFNUM);
}
static inline uint8_t modbus_if_getc(void){
    return usbcdc_getc(APP_MODBUSIF_INTFNUM);
}
static inline uint8_t modbus_if_read(void *buffer, uint8_t len){
    return usbcdc_read(APP_MODBUSIF_INTFNUM, buffer, len);
}
static inline uint8_t modbus_if_reqlock(uint8_t len){
    return usbcdc_reqlock(APP_MODBUSIF_INTFNUM, len, BYTEBUF_TOKEN_MODBUS);
}
static inline uint8_t modbus_if_rellock(void){
    return usbcdc_rellock(APP_MODBUSIF_INTFNUM, BYTEBUF_TOKEN_MODBUS);
}
static inline uint8_t modbus_if_putc(uint8_t byte){
    return usbcdc_putc(APP_MODBUSIF_INTFNUM, byte, BYTEBUF_TOKEN_MODBUS, 0x00);
}
static inline uint8_t modbus_if_write(void *buffer, uint8_t len){
    return usbcdc_write(APP_MODBUSIF_INTFNUM, buffer, len, BYTEBUF_TOKEN_MODBUS);
}
static inline void modbus_if_flush(void){
    usbcdc_send_flush(APP_MODBUSIF_INTFNUM);
    return;
}
#endif
#endif
#endif
