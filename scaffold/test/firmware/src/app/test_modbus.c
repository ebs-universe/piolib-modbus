

#include "test_modbus.h"
#include <ucdm/ucdm.h>


void modbus_test_init(void){
    // Initialize all UCDM registers
    // Some simple write access distribution, just enough to test the 
    // interface. All registers at addr = 5 and beyond have both register 
    // and bit write enabled.
    for (uint8_t i=5; i<DMAP_MAXREGS; i++){
        ucdm_enable_regw(i);
        ucdm_enable_bitw(i);
    }
    ucdm_exception_status = 0x80;;
}                        
