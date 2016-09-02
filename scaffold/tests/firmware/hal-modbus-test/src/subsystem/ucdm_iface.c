




#include "ucdm_iface.h"

uint8_t DMAP_MAXREGS = DMAP_MAX_REGISTERS;
ucdm_register_t ucdm_register[DMAP_MAX_REGISTERS];
uint8_t  ucdm_acctype[DMAP_MAX_REGISTERS];

void app_ucdm_init(void)
{
    ucdm_init();
    
    // Initialize all UCDM registers
    // Some simple write access distribution, just enough to test the 
    // interface. All registers at addr = 5 and beyond have both register 
    // and bit write enabled.
    for (uint8_t i=5; i<DMAP_MAXREGS; i++){
        ucdm_enable_regw(i);
        ucdm_enable_bitw(i);
    }
    ucdm_exception_status = 0x80;
    
}

