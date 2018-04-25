/* 
   Copyright (c)
     (c) 2018 Chintalagiri Shashank, Quazar Technologies Pvt. Ltd.
   
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
 * @file hotplug.h
 * @brief Modbus transport hotplug interface
 * 
 * @see hotplug.c
 */


#ifndef MODBUS_HOTPLUG_H
#define MODBUS_HOTPLUG_H

#if MODBUS_PLUGGABLE_TRANSPORTS == 1

#define MODBUS_HOTPLUG_EVENT_MASK   0xF8
#define MODBUS_HOTPLUG_EVENT_SIG    0x08

#if MODBUS_ENABLE_TRANSPORT_USBCDC
    #include "bsp/hal/uc/usbcdc.h"
    #define MODBUS_TRANSPORT_TAG_USBCDC     2
#endif
#if MODBUS_ENABLE_TRANSPORT_UART
    #include "bsp/hal/uc/uart.h"
    #define MODBUS_TRANSPORT_TAG_UART       1
#endif

uint16_t modbus_init_ptransports(uint16_t ucdm_address);
void modbus_install_transport(modbus_transport_t * mbtransport);
uint8_t modbus_switch_transport(uint8_t tag);
void modbus_hotplug_check(void);

#endif
#endif
