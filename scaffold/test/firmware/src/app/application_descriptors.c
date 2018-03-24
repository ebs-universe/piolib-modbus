

#include "application_descriptors.h"
#include <ucdm/descriptors.h>
#include "bsp/hal/uc/id.h"


const descriptor_vendor_t descriptor_vendor = {
    "Quazar Technologies Pvt Ltd",
    "http://www.quazartech.com",
};


const descriptor_hardware_t descriptor_hardware = {
    "HARDWARE_NAME",
    "HARDWARE_VARIANT",
    "HARDWARE_REVISION",
};


const descriptor_firmware_t descriptor_firmware = {
    "FIRMWARE_NAME",
    "FIRMWARE_REVISION",
};


descriptor_custom_t descriptor_mac = {0x00, 8, DESCRIPTOR_ACCTYPE_RFUNC, {&id_read}, NULL};


void application_descriptors_init(void){
    descriptor_install(&descriptor_mac);
}
