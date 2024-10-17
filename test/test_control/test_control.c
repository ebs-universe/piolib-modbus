
#include <unity.h>
#include <scaffold.h>
#include <ucdm/ucdm.h>
#include <modbus/modbus.h>

#define UCDM_MODBUS_ADDRESS             11

void test_ucdm_interface_configuration(void) {
    // Verify that the appropriate acctype and pointer is set
    ucdm_acctype_t acctype = ucdm_acctype[UCDM_MODBUS_ADDRESS];
    TEST_ASSERT_EQUAL(UCDM_AT_READ_PTR, acctype & UCDM_AT_READ_MASK);
    TEST_ASSERT_EQUAL(UCDM_AT_REGW_TYPE_PTR, acctype & UCDM_AT_REGW_MASK);
    TEST_ASSERT_EQUAL(&modbus_control, ucdm_register[UCDM_MODBUS_ADDRESS].ptr);
}

void test_device_address_read(void) {
    // Verify that device address can be read
    TEST_ASSERT_EQUAL(MODBUS_DEFAULT_DEVICE_ADDRESS, modbus_control.address);
}

void test_device_address_write(void) {
    // Verify that device address can be written
    modbus_set_address(7);
    TEST_ASSERT_EQUAL(7, modbus_control.address);
    modbus_set_address(5);
    TEST_ASSERT_EQUAL(MODBUS_DEFAULT_DEVICE_ADDRESS, modbus_control.address);
}

void test_device_address_read_ucdm(void) {
    // Verify that device address can be read through ucdm
    TEST_ASSERT_EQUAL(MODBUS_DEFAULT_DEVICE_ADDRESS, ucdm_get_register(UCDM_MODBUS_ADDRESS));
}

void test_device_address_write_ucdm(void) {
    // Verify that device address can be written throuh ucdm
    ucdm_set_register(UCDM_MODBUS_ADDRESS, 6);
    TEST_ASSERT_EQUAL(6, modbus_control.address);
    TEST_ASSERT_EQUAL(6, ucdm_get_register(UCDM_MODBUS_ADDRESS));
    ucdm_set_register(UCDM_MODBUS_ADDRESS, 5);
}

int main( int argc, char **argv) {
    init();
    UNITY_BEGIN();
    RUN_TEST(test_ucdm_interface_configuration);
    RUN_TEST(test_device_address_read);
    RUN_TEST(test_device_address_write);
    RUN_TEST(test_device_address_read_ucdm);
    RUN_TEST(test_device_address_write_ucdm);
    UNITY_END();
}
