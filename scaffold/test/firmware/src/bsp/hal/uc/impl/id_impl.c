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

#include "../id.h"
#include<string.h>


void id_init(void){
    ;
}


uint8_t id_read(uint8_t maxlen, void * buffer){
    HAL_BASE_t idlen;
    HAL_ADDRESS_t * idp;
    if (maxlen > uC_ID_LENGTH){
        maxlen = uC_ID_LENGTH;
    }
    TLV_getInfo(TLV_TAG_DIERECORD, 0, &idlen, &idp);
    if (idlen > maxlen){
        idlen = maxlen;
    }
    memcpy(buffer, idp, idlen);
    return(idlen);
}


uint8_t id_write(uint8_t len, uint8_t * content){
    return(0);
}
