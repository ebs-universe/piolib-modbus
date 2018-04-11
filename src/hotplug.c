

#include "modbus.h"
#include "hotplug.h"
#include "dispatch.h"
#include <ds/sllist.h>
#include <ucdm/ucdm.h>
#include "aduformat/uart.h"
#include "fcodes/common.h"
#include <string.h>


#if MODBUS_PLUGGABLE_TRANSPORTS == 1

modbus_transport_t * _mbtransports;
ucdm_register_t * _mb_current_transport;
uint16_t _mb_tr_addr;

#if MODBUS_ENABLE_TRANSPORT_UART == 1
modbus_transport_t modbus_transport_uart = {NULL, MODBUS_TRANSPORT_TAG_UART, 
    &ptransport_uart, &modbus_aduformat_uart, MODBUS_TRANSPORT_UART_INTFNUM};
#endif
#if MODBUS_ENABLE_TRANSPORT_USBCDC == 1
modbus_transport_t modbus_transport_usbcdc = {NULL, MODBUS_TRANSPORT_TAG_USBCDC, 
    &ptransport_usbcdc, &modbus_aduformat_uart, MODBUS_TRANSPORT_USBCDC_INTFNUM};
#endif

void modbus_install_transport(modbus_transport_t * mbtransport){
    mbtransport->transport->init(mbtransport->intfnum);
    sllist_install((void *)(&_mbtransports), (void *)mbtransport);
}

uint8_t modbus_switch_transport(uint8_t tag){
    modbus_transport_t * mbt = (modbus_transport_t *)sllist_find((void *)&_mbtransports, tag);
    if (mbt == NULL){
        return 1;
    }
    ucdm_register[_mb_tr_addr].data = (uint16_t)tag;
    modbus_sm.aduformat = mbt->aduformat;
    modbus_sm.intfnum = mbt->intfnum;
    modbus_sm.transport = mbt->transport;
    modbus_reset_sm();
    return 0;
}

uint16_t modbus_init_ptransports(uint16_t ucdm_address){
    _mb_tr_addr = ucdm_address;
    #if MODBUS_ENABLE_TRANSPORT_UART == 1
        modbus_install_transport(&modbus_transport_uart);
    #endif
    #if MODBUS_ENABLE_TRANSPORT_USBCDC == 1
        modbus_install_transport(&modbus_transport_usbcdc);
    #endif
    modbus_switch_transport(APP_MODBUS_TRANSPORT);
    return (ucdm_address + 1);
}

void modbus_hotplug_check(void){
    modbus_transport_t * mbt = _mbtransports; 
    uint8_t rlen, len;
    uint8_t rbyte[10];
    while(mbt){
        // Dont try to do anything on the transport currently in use 
        if (mbt->tag == ucdm_register[_mb_tr_addr].data) goto next_transport;

        // If there isn't anything waiting in the read buffer, move on
        rlen = mbt->transport->unhandled_rxb(mbt->intfnum);
        if (rlen == 0) goto next_transport;
        
        // If there isn't enough waiting in the read buffer, move on
        len = mbt->aduformat->prefix_n + mbt->aduformat->postfix_n + 5;
        if (rlen < len) goto next_transport;
        
        // If the message isn't intended for us, move on
        mbt->transport->read(mbt->intfnum, rbyte, len);
        if (rbyte[0] != *modbus_address_p) goto next_transport;
        
        // If the function code isn't 0x06, move on
        if (rbyte[mbt->aduformat->prefix_n] != MB_FC_WR_SREG) goto next_transport;
        
        // If the addressed register is not for the modbus transport, move on
        uint16_t wa = (uint16_t)rbyte[mbt->aduformat->prefix_n + 1] | (rbyte[mbt->aduformat->prefix_n + 2]);
        if (wa != _mb_tr_addr) goto next_transport;
        
        // If the crc is incorrect, move on
        uint16_t crc = modbus_calculate_crc(&(rbyte[0]), (len - 2));
        uint16_t apu_crc = (rbyte[len-2]) | ((uint16_t)(rbyte[len-1])<<8);
        if (crc != apu_crc) goto next_transport;
        
        // Execute the switch
        uint8_t result = modbus_switch_transport(mbt->tag);
        if (result == 0){
            // Build a valid response. 
            // TODO Make sure this works for other adu formats
            uint8_t resp_len = len - mbt->aduformat->postfix_n;
            modbus_sm.rxtxlen = resp_len;
            memcpy(&modbus_rxtxbuf[0], &rbyte[0], resp_len - 2);
            modbus_rxtxbuf[resp_len - 1] = ucdm_register[_mb_tr_addr].data;
            mbt->aduformat->pack();
            modbus_sm.state = MODBUS_ST_SEND;
        }
        next_transport: mbt = mbt->next;
    }
}

#endif
