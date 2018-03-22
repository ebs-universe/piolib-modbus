

#include "ucdm.h"

uint8_t DMAP_MAXREGS = DMAP_MAX_REGISTERS;
ucdm_register_t ucdm_register[DMAP_MAX_REGISTERS];
uint8_t  ucdm_acctype[DMAP_MAX_REGISTERS];

void app_ucdm_init(void)
{
    ucdm_init();
}

