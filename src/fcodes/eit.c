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
 * @file fcode_eit.c
 * @brief EIT Function Code Handler Implementations.
 * 
 * Device Identification Strings
 * 
 * 0x00 VendorName          descriptor_vendor.name
 * 0x01 ProductCode         descriptor_hardware.revision
 * 0x02 MajorMinorRevision  descriptor_firmware.revision
 * 0x03 VendorUrl           descriptor_vendor.url
 * 0x04 ProductName         descriptor_hardware.name
 * 0x05 ModelName           descriptor_hardware.variant
 * 0x06 UserApplicationName descriptor_firmware.name
 * 
 * Custom Objects
 * 0x80 DeviceMAC           id
 */

#include <ucdm/ucdm.h>
#include <ucdm/descriptors.h>
#include <string.h>

#include "../modbus.h"
#include "../dispatch.h"
#include "eit.h"


#define MEDI_CONFORMITY_LEVEL   0x83

#define MEDI_OID_VENDORNAME     0x00
#define MEDI_OID_PRODUCTCODE    0x01
#define MEDI_OID_MAJORMINORREV  0x02
#define MEDI_OID_VENDORURL      0x03
#define MEDI_OID_PRODUCTNAME    0x04
#define MEDI_OID_MODELNAME      0x05
#define MEDI_OID_APPLNAME       0x06
#define MEDI_OID_CUSTOMROOT     0x80
#define MEDI_OID_MAC            0x80

#define MEDI_EXC_MORE           0x00
#define MEDI_EXC_DONE           0xFD
#define MEDI_EXC_BADOID         0xFE


typedef struct _MODBUS_RDDEVID_CTRL_t{
    uint8_t oid;
    uint8_t loid;
    uint8_t * wp;
    uint8_t nobj;
} _modbus_rddevid_ctrl_t;

static _modbus_rddevid_ctrl_t _modbus_rddevid_ctrl;


static descriptor_custom_t * _findstart(uint8_t id){
    return(descriptor_find(id - 0x80));
}


static uint8_t _writecustomobjects(descriptor_custom_t * dptr){
    uint8_t id, len;
    while(1){
        id = dptr->tag + 0x80;
        if (id > _modbus_rddevid_ctrl.loid){
            return(MEDI_EXC_DONE);
        }
        if ((MODBUS_ADU_MAXLEN - modbus_sm.rxtxlen - 10) <= dptr->length){
            return(id);
        }
        *(_modbus_rddevid_ctrl.wp++) = id;
        *(_modbus_rddevid_ctrl.wp++) = dptr->length;
        len = descriptor_read(dptr, _modbus_rddevid_ctrl.wp);
        modbus_sm.rxtxlen += (len + 2);
        _modbus_rddevid_ctrl.wp += len;
        _modbus_rddevid_ctrl.nobj++;
        if (!dptr->next){
            return(MEDI_EXC_DONE);
        }
        dptr = dptr->next;
    };
}


static uint8_t _writeobject(uint8_t id, const char * sp){
    uint8_t len;
    if (id > _modbus_rddevid_ctrl.loid){
        return(MEDI_EXC_DONE);
    }
    len = (uint8_t) strlen(sp);
    if ((MODBUS_ADU_MAXLEN - modbus_sm.rxtxlen - 10) <= len){
        return(id);
    }
    *(_modbus_rddevid_ctrl.wp++) = id;
    *(_modbus_rddevid_ctrl.wp++) = len;
    memcpy(_modbus_rddevid_ctrl.wp, sp, len);
    modbus_sm.rxtxlen += (len + 2);
    _modbus_rddevid_ctrl.wp += len;
    _modbus_rddevid_ctrl.nobj++;  
    return(MEDI_EXC_MORE);
}


static uint8_t _modbus_rddevid_pushobjects(void){
    uint8_t error;
    descriptor_custom_t * dptr;
    switch(_modbus_rddevid_ctrl.oid){
        case(MEDI_OID_VENDORNAME):
            error = _writeobject(MEDI_OID_VENDORNAME, descriptor_vendor.name);
            if (error) return(error);
        case(MEDI_OID_PRODUCTCODE):
            error = _writeobject(MEDI_OID_PRODUCTCODE, descriptor_hardware.revision);
            if (error) return(error);
        case(MEDI_OID_MAJORMINORREV):
            error = _writeobject(MEDI_OID_MAJORMINORREV, descriptor_firmware.revision);
            if (error) return(error);
        case(MEDI_OID_VENDORURL):
            error = _writeobject(MEDI_OID_VENDORURL, descriptor_vendor.url);
            if (error) return(error);
        case(MEDI_OID_PRODUCTNAME):
            error = _writeobject(MEDI_OID_PRODUCTNAME, descriptor_hardware.name);
            if (error) return(error);
        case(MEDI_OID_MODELNAME):
            error = _writeobject(MEDI_OID_MODELNAME, descriptor_hardware.variant);
            if (error) return(error);
        case(MEDI_OID_APPLNAME):
            error = _writeobject(MEDI_OID_APPLNAME, descriptor_firmware.name);
            if (error) return(error);
        default:
            if (_modbus_rddevid_ctrl.loid == 0x7F){
                return(MEDI_EXC_DONE);
            }
            if (_modbus_rddevid_ctrl.nobj){
                error = MEDI_OID_CUSTOMROOT;
            }
            else if (_modbus_rddevid_ctrl.oid < MEDI_OID_CUSTOMROOT){
                return(MEDI_EXC_BADOID);
            }
            else{
                error = _modbus_rddevid_ctrl.oid;
            }
            dptr = _findstart(error);
            if (!dptr) return(MEDI_EXC_BADOID);
            return (_writecustomobjects(dptr));
    };
}


static void _modbus_handler_eit_rddevid(void){
    uint8_t rdicode = MODBUS_RBYTE(2);
    uint8_t error;
    if (modbus_ctrans.broadcast){
        return;   
    }
    _modbus_rddevid_ctrl.oid = MODBUS_RBYTE(3);
    _modbus_rddevid_ctrl.nobj = 0;
    _modbus_rddevid_ctrl.wp = &modbus_rxtxbuf[modbus_sm.aduformat->prefix_n + 3];
    *(_modbus_rddevid_ctrl.wp++) = MEDI_CONFORMITY_LEVEL;
    modbus_sm.rxtxlen = modbus_sm.aduformat->prefix_n + 7;
    switch(rdicode){
        case (0x01):
            _modbus_rddevid_ctrl.loid = 0x02;
            break;
        case (0x02):
            _modbus_rddevid_ctrl.loid = 0x7F;
            break;
        case (0x03):
            _modbus_rddevid_ctrl.loid = 0xFF;
            break;
        case (0x04):
            *(_modbus_rddevid_ctrl.wp++) = 0x00;
            *(_modbus_rddevid_ctrl.wp++) = 0x00;
            *(_modbus_rddevid_ctrl.wp++) = 0x01;
            error = _modbus_rddevid_pushobjects();
            if (error == MEDI_EXC_BADOID) {
                modbus_build_exc_response(0x02);
                return;
            }
            modbus_sm.aduformat->pack();
            return;
        default:
            modbus_build_exc_response(0x03);
            return;
    };
    _modbus_rddevid_ctrl.wp += 3;
    error = _modbus_rddevid_pushobjects();
    if (error == MEDI_EXC_BADOID){
        _modbus_rddevid_ctrl.oid = 0x00;
        error = _modbus_rddevid_pushobjects();
    }
    _modbus_rddevid_ctrl.wp = &modbus_rxtxbuf[modbus_sm.aduformat->prefix_n + 4];
    if (error == MEDI_EXC_DONE){
        *(_modbus_rddevid_ctrl.wp++) = 0x00;
        *(_modbus_rddevid_ctrl.wp++) = 0x00;
    }
    else if (error){
        *(_modbus_rddevid_ctrl.wp++) = 0xFF;
        *(_modbus_rddevid_ctrl.wp++) = error;
    }
    *(_modbus_rddevid_ctrl.wp++) = _modbus_rddevid_ctrl.nobj;
    modbus_sm.aduformat->pack();
}


void modbus_handler_eit(void)
{
    uint8_t meitype = MODBUS_RBYTE(1);
    switch(meitype){
        case(MB_SFC_EIT_RDDEVID):
            return(_modbus_handler_eit_rddevid());
        default:
            return(modbus_handler_notimpl());
    }
}


const modbus_fcode_handler_t _eit_handler = {
    MB_FC_EIT,
    2, 0,
    #if MB_SUPPORT_FC_EIT
        &modbus_handler_eit,
    #else
        &modbus_handler_notimpl,
    #endif
};
