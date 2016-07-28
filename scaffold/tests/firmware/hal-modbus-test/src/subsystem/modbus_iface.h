


#ifndef MODBUS_IFACE_H
#define MODBUS_IFACE_H

#include"board_pum.h"
#include"application.h"
#include"hal_uc.h"

#define BYTEBUF_TOKEN_MODBUS    0x06

void modbus_if_init(void);

uint8_t modbus_if_unhandled_rxb(void);
uint8_t modbus_if_getc(void);
uint8_t modbus_if_read(void *buffer, uint8_t len);

uint8_t modbus_if_reqlock(uint8_t len);
uint8_t modbus_if_rellock(void);
uint8_t modbus_if_putc(uint8_t byte);
uint8_t modbus_if_write(void *buffer, uint8_t len);


#endif