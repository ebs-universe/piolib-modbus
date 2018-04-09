

#include "modbus.h"
#include "hotplug.h"
#include <ds/sllist.h>

#include "aduformat/uart.h"


#if MODBUS_PLUGGABLE_TRANSPORTS == 1

modbus_transport_t * _mbtransports;

#if MODBUS_ENABLE_TRANSPORT_UART == 1
modbus_transport_t modbus_transport_uart = {NULL, 0x01, 
    &ptransport_uart, &modbus_aduformat_uart, MODBUS_TRANSPORT_UART_INTFNUM};
#endif
#if MODBUS_ENABLE_TRANSPORT_USBCDC == 1
modbus_transport_t modbus_transport_usbcdc = {NULL, 0x02, 
    &ptransport_usbcdc, &modbus_aduformat_uart, MODBUS_TRANSPORT_USBCDC_INTFNUM};
#endif

void modbus_init_ptransports(void){
    #if MODBUS_ENABLE_TRANSPORT_UART == 1
        modbus_install_transport(&modbus_transport_uart);
    #endif
    #if MODBUS_ENABLE_TRANSPORT_USBCDC == 1
        modbus_install_transport(&modbus_transport_usbcdc);
    #endif
}

void modbus_install_transport(modbus_transport_t * mbtransport){
    sllist_install((void *)&_mbtransports, (void *)mbtransport);
}

void modbus_switch_transport(uint8_t tag){
    modbus_transport_t * mbt = (modbus_transport_t *)sllist_find((void *)&_mbtransports, tag);
    modbus_sm.aduformat = mbt->aduformat;
    modbus_sm.intfnum = mbt->intfnum;
    modbus_sm.transport = mbt->transport;
    modbus_reset_sm();
}

#endif
