CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
CMAKE_POLICY(SET CMP0007 NEW)


PROJECT(hal-modbus-test C)
SET(hal-modbus-test-hal_VERSION_MAJOR 0)
SET(hal-modbus-test-hal_VERSION_MINOR 1)
SET(hal-modbus-test-hal_VERSION_PATCH 0)
SET(hal-modbus-test-hal_VERSION_TWEAK 0)
SET(TMP_PROJECT_DESCRIPTION "MODBUS test firmware built on top of the Hardware Abstraction Layers"
        CACHE STRING "Project Description")

SET(DOC_HTML_INSTALL	True)

INCLUDE(doc/instance.cmake)

INCLUDE_DIRECTORIES("${CMAKE_SOURCE_DIR} ${INCLUDE_DIRECTORIES}")

ADD_SUBDIRECTORY(doc)
ADD_SUBDIRECTORY(bsp)
ADD_SUBDIRECTORY(sys)
ADD_SUBDIRECTORY(app)

INSTALL(SCRIPT default-install.cmake)
