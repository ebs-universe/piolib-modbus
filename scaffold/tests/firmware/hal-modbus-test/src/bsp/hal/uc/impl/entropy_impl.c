/* 
   Copyright 2015 Quazar Technologies Pvt. Ltd.
   Copyright 2015 Chintalagiri Shashank
   
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


#include "../entropy.h"
#include "../timer.h"
#include "app/application.h"
#include <msp430-driverlib/MSP430F5xx_6xx/driverlib.h>
#include <stdlib.h>

typedef struct{
    uint16_t ucsctl4_bak;
    uint16_t taXctl_bak;
    uint16_t taXcctl2_bak;
} entropy_state_t;

static entropy_state_t * entropy_state;

#define CTLREG  HWREG16(timer_if[APP_ENTROPY_INTFNUM]->hwif->base + OFS_TAxCTL)
#define CCTLREG HWREG16(timer_if[APP_ENTROPY_INTFNUM]->hwif->base + OFS_TAxCCTL2)
#define CCRREG  HWREG16(timer_if[APP_ENTROPY_INTFNUM]->hwif->base + OFS_TAxCCR2)


void entropy_init(void){
    entropy_state = malloc(sizeof(entropy_state_t));

    // Switch ACLK to VLO
    entropy_state->ucsctl4_bak = UCSCTL4;
    UCSCTL4 &= ~(SELA_7);
    UCSCTL4 |= UCS_VLOCLK_SELECT << 8;
    
    // Configure TimerA0
    entropy_state->taXctl_bak = CTLREG;
    entropy_state->taXcctl2_bak = CCTLREG;
    
    CCTLREG = CAP | CM_1 | CCIS_1;
    CTLREG = TASSEL__SMCLK | MC__CONTINUOUS;    
}

uint8_t entropy_get_byte(void){
    uint8_t i, j, result, ones;
    result = 0;
    for (i = 0; i < 8; i++) {
        ones = 0;
        for (j = 0; j < 5; j++) {
            while (!(CCIFG & CCTLREG));     // Wait for interrupt
            CCTLREG &= ~CCIFG;              // Clear interrupt
            if (1 & CCRREG)                 // If LSb set, count it
                ones++;
            }
        result >>= 1;                       // Save previous bits
        if (ones >= 3)                      // Best out of 5
            result |= 0x80;                 // Set MSb
    }
    return result;
}

void entropy_deinit(void){
    CCTLREG = entropy_state->taXcctl2_bak;
    CTLREG = entropy_state->taXctl_bak;
    UCSCTL4 = entropy_state->ucsctl4_bak;
    free(entropy_state);
}
