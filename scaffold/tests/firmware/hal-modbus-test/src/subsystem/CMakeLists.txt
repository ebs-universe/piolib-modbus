
FIND_PACKAGE("modbus" REQUIRED)

ADD_PLATFORM_LIBRARY(modbus-if STATIC "hal-uc-uart modbus" modbus_iface.c)
