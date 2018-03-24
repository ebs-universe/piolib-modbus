/* 
 Copyright 2018 Quazar Technologies Pvt. Ltd.
 Copyright 2018 Chintalagiri Shashank
 
 This file is part of
 Embedded bootstraps : Peripheral driver implementations : MSP430
 
 This library is free software: you can redistribute it and/or modify
 it under the terms of the GNU Lesser General Public License as published
 by the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU Lesser General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>. 
 */

#ifndef ID_IMPL_H
#define ID_IMPL_H

#include "app/application.h"
#include "../id.h"

#include "msp430-driverlib/MSP430F5xx_6xx/driverlib.h"

/**
 * MSP430 ID is constructed from the DIERECORD TLV, including :
 *   - 4 bytes : Lot / Wafer ID
 *   - 2 bytes : Die X Position
 *   - 2 bytes : Die Y Position
 * An additional 2 bytes are included in the DIERECORD containing Test Results. 
 * These bytes are not used. 
 */
#define uC_ID_LENGTH    8

#endif
