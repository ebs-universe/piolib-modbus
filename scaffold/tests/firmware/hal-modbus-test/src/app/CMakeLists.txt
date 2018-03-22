
FIND_PACKAGE("prbs" REQUIRED)
FIND_PACKAGE("ucdm" REQUIRED)
FIND_PACKAGE("modbus" REQUIRED)
FIND_PACKAGE("time" REQUIRED)
FIND_PACKAGE("ds" REQUIRED)
FIND_PACKAGE("uuid" REQUIRED)
FIND_PACKAGE("crypto" REQUIRED)
FIND_PACKAGE("printf" REQUIRED)


ADD_PLATFORM_LIBRARY(test-modbus STATIC "ucdm" test_modbus.c)
ADD_PLATFORM_LIBRARY(application-descriptors STATIC "ucdm hal-uc-id" application_descriptors.c)
SET(application_SRCS main.c)
LIST(APPEND deps "time uuid hal-uc-core hal-uc-gpio hal-uc-uart hal-uc-timer modbus msp430-usbapi modbus-if ucdm-if time-if application-descriptors test-modbus")
SET(EXTRA_LINKER_FLAGS "-T ${CMAKE_SOURCE_DIR}/bsp/hal/uc/impl/usb-impl/USB_API/msp430USB.ld")
ADD_PLATFORM_EXECUTABLE(firmware ${deps} ${application_SRCS})
