
FIND_PACKAGE("modbus" REQUIRED)
FIND_PACKAGE("time" REQUIRED)
FIND_PACKAGE("ucdm" REQUIRED)

ADD_PLATFORM_LIBRARY(modbus-if STATIC "hal-uc-uart modbus" modbus_iface.c)
ADD_PLATFORM_LIBRARY(time-if STATIC "hal-uc-timer time" time_iface.c)
ADD_PLATFORM_LIBRARY(ucdm-if STATIC "ucdm" ucdm_iface.c)
