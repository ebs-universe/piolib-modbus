

#include <platform/types.h>


void sysinit(void); 
void libinit(void);
void init(void);

void modbus_if_init(void);
HAL_BASE_t modbus_if_unhandled_rxb(void);
HAL_BASE_t modbus_if_read(uint8_t *buffer, HAL_BASE_t len);
HAL_BASE_t modbus_if_reqlock(HAL_BASE_t len);
HAL_BASE_t modbus_if_rellock(void);
HAL_BASE_t modbus_if_write(uint8_t *buffer, uint8_t len);
void modbus_if_flush(void);
