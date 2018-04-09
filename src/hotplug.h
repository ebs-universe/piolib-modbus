




#ifndef MODBUS_HOTPLUG_H
#define MODBUS_HOTPLUG_H

#if MODBUS_PLUGGABLE_TRANSPORTS == 1

#if MODBUS_ENABLE_TRANSPORT_USBCDC
    #include "bsp/hal/uc/usbcdc.h"
#endif
#if MODBUS_ENABLE_TRANSPORT_UART
    #include "bsp/hal/uc/uart.h"
#endif

void modbus_init_ptransports(void);
void modbus_install_transport(modbus_transport_t * mbtransport);
void modbus_switch_transport(uint8_t tag);

#endif
#endif
