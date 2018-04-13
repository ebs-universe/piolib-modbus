
FIND_PACKAGE("prbs" REQUIRED)
FIND_PACKAGE("ucdm" REQUIRED)
FIND_PACKAGE("time" REQUIRED)
FIND_PACKAGE("ds" REQUIRED)
FIND_PACKAGE("uuid" REQUIRED)
FIND_PACKAGE("crypto" REQUIRED)
FIND_PACKAGE("printf" REQUIRED)


SET(application_SRCS main.c
                     application_descriptors.c
                     test_modbus.c)
SET(deps "${deps} hal-uc-core hal-uc-gpio hal-uc-uart hal-uc-id msp430-usbapi") 
SET(deps "${deps} modbus ucdm ucdm-if time-if time uuid rand")
SET(EXTRA_LINKER_FLAGS "-T ${CMAKE_SOURCE_DIR}/bsp/hal/uc/impl/usb-impl/USB_API/msp430USB.ld")
ADD_PLATFORM_EXECUTABLE(firmware ${deps} ${application_SRCS})
