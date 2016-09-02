
FIND_PACKAGE("prbs" REQUIRED)
FIND_PACKAGE("ucdm" REQUIRED)
FIND_PACKAGE("modbus" REQUIRED)
FIND_PACKAGE("time" REQUIRED)
FIND_PACKAGE("ds" REQUIRED)

SET(application_SRCS main.c)
LIST(APPEND deps "hal-uc-core hal-uc-gpio hal-uc-uart hal-uc-timer prbs modbus modbus-if ucdm ucdm-if time time-if msp430-usbapi ds")
SET(EXTRA_LINKER_FLAGS "-T ${CMAKE_SOURCE_DIR}/uc-impl/usb-impl/USB_API/msp430USB.ld")
ADD_PLATFORM_EXECUTABLE(firmware ${deps} ${application_SRCS})
