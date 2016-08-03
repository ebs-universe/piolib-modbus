
FIND_PACKAGE("prbs" REQUIRED)
FIND_PACKAGE("ucdm" REQUIRED)
FIND_PACKAGE("modbus" REQUIRED)
FIND_PACKAGE("time" REQUIRED)

SET(application_SRCS main.c)
LIST(APPEND deps "hal-uc-core hal-uc-gpio hal-uc-uart hal-uc-timer prbs modbus modbus-if ucdm ucdm-if time time-if")
ADD_PLATFORM_EXECUTABLE(firmware ${deps} ${application_SRCS})
