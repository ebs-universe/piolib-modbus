


#ifndef MODBUS_IFACE_H
#define MODBUS_IFACE_H

#include"app/application.h"
#include"bsp/hal/uc.h"
#include <modbus/modbus.h>

#define BYTEBUF_TOKEN_MODBUS    0x06

extern uint16_t *const modbus_diagnostic_register_p;
extern uint8_t  *const modbus_exception_status_p;

static inline void app_modbus_init(void){
    modbus_init(UCDM_MODBUS_BASE_ADDRESS, MODBUS_DEFAULT_DEVICE_ADDRESS);
}

void modbus_if_init(void);

uint8_t modbus_if_unhandled_rxb(void);
uint8_t modbus_if_getc(void);
uint8_t modbus_if_read(void *buffer, uint8_t len);

uint8_t modbus_if_reqlock(uint8_t len);
uint8_t modbus_if_rellock(void);
uint8_t modbus_if_putc(uint8_t byte);
uint8_t modbus_if_write(void *buffer, uint8_t len);
void modbus_if_flush(void);

#endif
