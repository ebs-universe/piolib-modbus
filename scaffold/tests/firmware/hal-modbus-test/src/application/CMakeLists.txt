
FIND_PACKAGE("ucdm" REQUIRED)
FIND_PACKAGE("modbus" REQUIRED)

SET(application_SRCS main.c)
LIST(APPEND deps "hal-uc-core hal-uc-gpio hal-uc-uart modbus modbus-if ucdm")
ADD_PLATFORM_EXECUTABLE(firmware ${deps} ${application_SRCS})
