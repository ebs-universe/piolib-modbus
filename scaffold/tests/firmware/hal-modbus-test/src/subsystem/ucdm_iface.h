



#ifndef UCDM_IFACE_H
#define UCDM_IFACE_H

#include"application.h"
#include"hal_uc.h"

extern uint16_t ucdm_diagnostic_register;
extern uint8_t  ucdm_exception_status;


void app_ucdm_init(void);

#endif