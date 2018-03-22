



#include "modbus.h"
#include "ucdm.h"

uint16_t *const modbus_diagnostic_register_p = &ucdm_diagnostic_register;
uint8_t  *const modbus_exception_status_p = &ucdm_exception_status;
const uint8_t modbus_if_rxbuf_chunksize = APP_MODBUSIF_RXCHUNKSIZE;
const uint8_t modbus_if_txbuf_chunksize = APP_MODBUSIF_TXCHUNKSIZE;

#if APP_MODBUSIF_TYPE == MODBUS_UART
void modbus_if_init(void){
    uart_if[APP_MODBUSIF_INTFNUM]->state->overrun_counter = &modbus_bus_char_overrun_cnt;
    uart_init(APP_MODBUSIF_INTFNUM);
}
uint8_t modbus_if_unhandled_rxb(void){
    return uart_population_rxb(APP_MODBUSIF_INTFNUM);
}
uint8_t modbus_if_getc(void){
    return uart_getc(APP_MODBUSIF_INTFNUM);
}
uint8_t modbus_if_read(void *buffer, uint8_t len){
    return uart_read(APP_MODBUSIF_INTFNUM, buffer, len);
}
uint8_t modbus_if_reqlock(uint8_t len){
    return uart_reqlock(APP_MODBUSIF_INTFNUM, len, BYTEBUF_TOKEN_MODBUS);
}
uint8_t modbus_if_rellock(void){
    return uart_rellock(APP_MODBUSIF_INTFNUM, BYTEBUF_TOKEN_MODBUS);
}
uint8_t modbus_if_putc(uint8_t byte){
    return uart_putc(APP_MODBUSIF_INTFNUM, byte, BYTEBUF_TOKEN_MODBUS, 0x00);
}
uint8_t modbus_if_write(void *buffer, uint8_t len){
    return uart_write(APP_MODBUSIF_INTFNUM, buffer, len, BYTEBUF_TOKEN_MODBUS);
}
void modbus_if_flush(void){
    uart_send_flush(APP_MODBUSIF_INTFNUM);
    return;
}
#elif APP_MODBUSIF_TYPE == MODBUS_USBCDC
void modbus_if_init(void){
    return;
}
uint8_t modbus_if_unhandled_rxb(void){
    return usbcdc_population_rxb(APP_MODBUSIF_INTFNUM);
}
uint8_t modbus_if_getc(void){
    return usbcdc_getc(APP_MODBUSIF_INTFNUM);
}
uint8_t modbus_if_read(void *buffer, uint8_t len){
    return usbcdc_read(APP_MODBUSIF_INTFNUM, buffer, len);
}
uint8_t modbus_if_reqlock(uint8_t len){
    return usbcdc_reqlock(APP_MODBUSIF_INTFNUM, len, BYTEBUF_TOKEN_MODBUS);
}
uint8_t modbus_if_rellock(void){
    return usbcdc_rellock(APP_MODBUSIF_INTFNUM, BYTEBUF_TOKEN_MODBUS);
}
uint8_t modbus_if_putc(uint8_t byte){
    return usbcdc_putc(APP_MODBUSIF_INTFNUM, byte, BYTEBUF_TOKEN_MODBUS, 0x00);
}
uint8_t modbus_if_write(void *buffer, uint8_t len){
    return usbcdc_write(APP_MODBUSIF_INTFNUM, buffer, len, BYTEBUF_TOKEN_MODBUS);
}
void modbus_if_flush(void){
    usbcdc_send_flush(APP_MODBUSIF_INTFNUM);
    return;
}
#endif
