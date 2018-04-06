/* 
 * Copyright (c)
 *   (c) 2015-2016 Chintalagiri Shashank, Quazar Technologies Pvt. Ltd.
 *  
 * This file is part of
 * Embedded bootstraps : Peripheral driver implementations : MSP430
 * 
 * This library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>. 
 */

/**
 * @file entropy_impl.h
 * @brief MSP430 Entropy Implementation Notes
 *
 * Entropy accumulation for MSP430 can potentially be done using the following
 * sources :
 * 
 *   - ADC Conversion of temperature sensor. 
 *   - ADC Conversion of Vmid. 
 *   - Clock variation between SMCLK and ACLK.
 * 
 * This implementation is based on SLAA338, and utilizes the MSP430's SMCLK, 
 * ACLK sourced from VLO, and a TIMERA. The timer to use should be defined in 
 * the application configuration (APP_ENTROPY_INTFNUM). 
 * 
 * This implementation uses the definitions for the MSP430F552x, and may not
 * port directly to other MSP430s. Specifically, check the CCI channel in the
 * uC datasheet and ensure the correct channel (providing ACLK) is used here.
 * 
 * Note that this will destroy the accuracy of any timekeeping mechanism 
 * which depends on ACLK, including that of the on-chip RTC. You should also 
 * expect unusual behavior from any other subsystems which use the same TIMER.
 * 
 * @see https://github.com/0/msp430-rng
 * @see http://www.ti.com/lit/an/slaa338/slaa338.pdf
 * 
 */


#ifndef ENTROPY_IMPL_H
#define ENTROPY_IMPL_H


#endif
