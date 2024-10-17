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
    
#include "config.h"
#include "modbus.h"

#define MODBUS_TRANSPORT_TYPE_UART      0
#define MODBUS_TRANSPORT_TYPE_USBCDC    1
#define MODBUS_TRANSPORT_TYPE_APP       0xFE

#if MODBUS_PLUGGABLE_TRANSPORTS == 1

static inline void modbus_if_init(void){
    // TODO Figure out overrun counter installation
    return modbus_sm.transport->init(modbus_sm.intfnum);
}

static inline uint8_t modbus_if_unhandled_rxb(void){
    return modbus_sm.transport->unhandled_rxb(modbus_sm.intfnum);
}

static inline uint8_t modbus_if_read(void *buffer, uint8_t len){
    return modbus_sm.transport->read(modbus_sm.intfnum, buffer, len);
}

static inline uint8_t modbus_if_reqlock(uint8_t len){
    return modbus_sm.transport->reqlock(modbus_sm.intfnum, len, BYTEBUF_TOKEN_MODBUS);
}

static inline uint8_t modbus_if_rellock(void){
    return modbus_sm.transport->rellock(modbus_sm.intfnum, BYTEBUF_TOKEN_MODBUS);
}

static inline uint8_t modbus_if_write(void *buffer, uint8_t len){
    return modbus_sm.transport->write(modbus_sm.intfnum, buffer, len, BYTEBUF_TOKEN_MODBUS);
}

static inline void modbus_if_flush(void){
    return modbus_sm.transport->flush(modbus_sm.intfnum);
}

#else

#if MODBUS_TRANSPORT_TYPE == MODBUS_TRANSPORT_TYPE_APP

/**
 * @name Modbus Application Dependent External API Functions
 * 
 * These functions related to hardware or application-specific interfaces and 
 * must be provided by the application. 
 * 
 * The function declarations are written to be transport and platform agnostic. 
 * An unfortunate side-effect of this is that the functions will have to be 
 * real ones and cannot simply be inlined. This will result in an unavoidable 
 * performance hit which is not presently quantified.
 * 
 * The modbus library implemented here first copies out as many bytes as it 
 * can into a modbus library buffer, within which the commmand is processed 
 * and handled. 
 * 
 * In applications requiring higher modbus command throughput, this intermediate 
 * buffer should be bypassed and the modbus commands handled directly within 
 * the bytebuf. The best approach to do this would be for the application to 
 * provide (and use) it's own modbus state machine implementation, using the 
 * sources for the one implemented here as a guide.
 * 
 * The implementation here does not use this approach to avoid the assumption
 * of the presence of bytebufs at the underlying hardware interface driver. 
 * This assumption would probably make the implementation incompatible with 
 * regular buffers or other interface types. 
 * 
 * This way, the protocol library is well separated from the hardware driver 
 * and we rely on the HAL and subsystem layers to sort out how to perform 
 * specific, well defined tasks and just link against them.
 * 
 */
/**@{*/ 
void modbus_if_init(void);
HAL_BASE_t modbus_if_unhandled_rxb(void);
HAL_BASE_t modbus_if_read(uint8_t *buffer, HAL_BASE_t len);
HAL_BASE_t modbus_if_reqlock(HAL_BASE_t len);
HAL_BASE_t modbus_if_rellock(void);
HAL_BASE_t modbus_if_write(uint8_t *buffer, uint8_t len);
void modbus_if_flush(void);
/**@}*/ 

#elif MODBUS_TRANSPORT_TYPE == MODBUS_TRANSPORT_TYPE_UART

#include <hal/uc/uart.h>

static inline void modbus_if_init(void){
    // Overrun counter is contained within the uart_if state now. We need to 
    // get modbus to read from there instead. 
    // uart_if[MODBUS_TRANSPORT_INTFNUM]->state->overrun_counter = &modbus_bus_char_overrun_cnt;
    
    // The current structure of the EBS application and startup separates configuration
    // of the interface, and it will get run because of APP_ENABLE_PRIF / APP_ENABLE_BCIF, 
    // for example. This approach was already used for USBCDC earlier. We leave modbus_if_init
    // in place in case there are special cases, but for the moment we leave it empty.
    // uart_init(MODBUS_TRANSPORT_INTFNUM);
    return;
}
static inline HAL_BASE_t modbus_if_unhandled_rxb(void){
    return uart_population_rxb(MODBUS_TRANSPORT_INTFNUM);
}
static inline HAL_BASE_t modbus_if_read(uint8_t *buffer, HAL_BASE_t len){
    return uart_read(MODBUS_TRANSPORT_INTFNUM, buffer, len);
}
static inline HAL_BASE_t modbus_if_reqlock(HAL_BASE_t len){
    return uart_reqlock(MODBUS_TRANSPORT_INTFNUM, len, BYTEBUF_TOKEN_MODBUS);
}
static inline HAL_BASE_t modbus_if_rellock(void){
    return uart_rellock(MODBUS_TRANSPORT_INTFNUM, BYTEBUF_TOKEN_MODBUS);
}
static inline HAL_BASE_t modbus_if_write(uint8_t *buffer, HAL_BASE_t len){
    return uart_write(MODBUS_TRANSPORT_INTFNUM, buffer, len, BYTEBUF_TOKEN_MODBUS);
}
static inline void modbus_if_flush(void){
    uart_send_flush(MODBUS_TRANSPORT_INTFNUM);
    return;
}
#elif MODBUS_TRANSPORT_TYPE == MODBUS_TRANSPORT_TYPE_USBCDC

#include <hal/uc/usbcdc.h>

static inline void modbus_if_init(void){
    return;
}
static inline HAL_BASE_t modbus_if_unhandled_rxb(void){
    return usbcdc_population_rxb(MODBUS_TRANSPORT_INTFNUM);
}
static inline HAL_BASE_t modbus_if_read(uint8_t *buffer, uint8_t len){
    return usbcdc_read(MODBUS_TRANSPORT_INTFNUM, buffer, len);
}
static inline HAL_BASE_t modbus_if_reqlock(uint8_t len){
    return usbcdc_reqlock(MODBUS_TRANSPORT_INTFNUM, len, BYTEBUF_TOKEN_MODBUS);
}
static inline HAL_BASE_t modbus_if_rellock(void){
    return usbcdc_rellock(MODBUS_TRANSPORT_INTFNUM, BYTEBUF_TOKEN_MODBUS);
}
static inline HAL_BASE_t modbus_if_write(uint8_t *buffer, uint8_t len){
    return usbcdc_write(MODBUS_TRANSPORT_INTFNUM, buffer, len, BYTEBUF_TOKEN_MODBUS);
}
static inline void modbus_if_flush(void){
    usbcdc_send_flush(MODBUS_TRANSPORT_INTFNUM);
    return;
}
#endif

#endif
#endif
