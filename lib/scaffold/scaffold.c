
#include "scaffold.h"
#include <ucdm/descriptor.h>
#include <modbus/modbus.h>
#include <time/time.h>


void setUp(void) {
    // setup stuff here
}

void tearDown(void) {
    // clean stuff up here
}

const descriptor_vendor_t descriptor_vendor = {
    .name = "EBS Development",
    .url = "https://github.com/ebs-universe"
};

const descriptor_hardware_t descriptor_hardware = {
    .name = "Native",
    .revision = "9.9.9",
    .variant = ""
};

const descriptor_firmware_t descriptor_firmware = {
    .name = "Modbus Test Scaffold",
    .revision = "9.9.9"
};


void modbus_if_init(void){
    ;
}

HAL_BASE_t modbus_if_unhandled_rxb(void){
    return 0;
}

HAL_BASE_t modbus_if_read(uint8_t *buffer, HAL_BASE_t len){
    return 0;
}

HAL_BASE_t modbus_if_reqlock(HAL_BASE_t len){
    return 0;
}

HAL_BASE_t modbus_if_rellock(void){
    return 0;
}

HAL_BASE_t modbus_if_write(uint8_t *buffer, uint8_t len){
    return 0;
}

void modbus_if_flush(void){
    ;
}

void sysinit(void) {

}

void libinit(void) {
    ucdm_addr_t addr = tm_init(1);    
    modbus_init(addr, 0x05);
}

void init(void) {
    sysinit();
    libinit();
}
