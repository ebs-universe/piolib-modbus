

#include "test_modbus.h"
#include <ucdm/ucdm.h>
#include <string.h>
#include "sys/modbus/modbus.h"


void modbus_test_reset_field(uint16_t addr, uint16_t value);

void modbus_test_reset_field(uint16_t __attribute__((unused)) addr, uint16_t __attribute__((unused)) value){
    memset(&(ucdm_register[5]), 0, sizeof(ucdm_register_t) * (DMAP_MAXREGS - 4));
    modbus_sm.silent = MODBUS_OUT_NORMAL;
}


void modbus_test_init(void){
    // Initialize all UCDM registers with some simple write access distribution, 
    // just enough to test the interface. 
    ucdm_redirect_regw_func(DMAP_MODBUS_TEST_BASE_ADDRESS, &modbus_test_reset_field);
    for (uint8_t i=DMAP_MODBUS_TEST_BASE_ADDRESS + 1; i<DMAP_MAXREGS; i++){
        if ( i%5 != 0 ){
            ucdm_enable_regw(i);
        }
        if ( i%3 != 0){
            ucdm_enable_bitw(i);
        }
    }
    ucdm_exception_status = 0x80;
    ucdm_diagnostic_register = 0x6654;
}                        
