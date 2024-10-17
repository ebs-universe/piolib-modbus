#include "unity.h"
#include <modbus/modbus.h>
#include <scaffold.h>


// Test CRC for a single-byte command
void test_modbus_calculate_crc_single_byte(void) {
    uint8_t cmd[] = {0x01};  // Example MODBUS ADU with one byte (address byte).
    uint16_t expected_crc = 0x807E;  // Pre-calculated CRC for this input.
    
    uint16_t crc = modbus_calculate_crc(cmd, sizeof(cmd));

    TEST_ASSERT_EQUAL_HEX16(expected_crc, crc);
}

// Test CRC for a two-byte command
void test_modbus_calculate_crc_two_bytes(void) {
    uint8_t cmd[] = {0x01, 0x03};  // Example MODBUS ADU with address and function bytes.
    uint16_t expected_crc = 0x2140;  // Pre-calculated CRC for this input.

    uint16_t crc = modbus_calculate_crc(cmd, sizeof(cmd));

    TEST_ASSERT_EQUAL_HEX16(expected_crc, crc);
}

// Test CRC for a typical MODBUS command with 8 bytes
void test_modbus_calculate_crc_eight_bytes(void) {
    uint8_t cmd[] = {0x01, 0x03, 0x00, 0x00, 0x00, 0x02, 0xC4, 0x0B};  // Typical MODBUS request.
    uint16_t expected_crc = 0x0000;  // Pre-calculated CRC for this input.

    uint16_t crc = modbus_calculate_crc(cmd, sizeof(cmd));

    TEST_ASSERT_EQUAL_HEX16(expected_crc, crc);
}

// Test CRC for a longer command with 16 bytes
void test_modbus_calculate_crc_sixteen_bytes(void) {
    uint8_t cmd[] = {0x01, 0x10, 0x00, 0x01, 0x00, 0x02, 0x04, 0x00, 
                     0xA1, 0x00, 0xB2, 0x00, 0x00, 0x00, 0x00, 0x00};
    uint16_t expected_crc = 0xE6F7;  // Pre-calculated CRC for this input.

    uint16_t crc = modbus_calculate_crc(cmd, sizeof(cmd));

    TEST_ASSERT_EQUAL_HEX16(expected_crc, crc);
}

// Test CRC for an empty buffer
void test_modbus_calculate_crc_empty_buffer(void) {
    uint8_t cmd[] = {};
    uint16_t expected_crc = 0xFFFF;  // MODBUS CRC of empty command is generally considered 0xFFFF.

    uint16_t crc = modbus_calculate_crc(cmd, sizeof(cmd));

    TEST_ASSERT_EQUAL_HEX16(expected_crc, crc);
}

// Test CRC for random data (length of 5 bytes)
void test_modbus_calculate_crc_random_data(void) {
    uint8_t cmd[] = {0x12, 0x34, 0x56, 0x78, 0x9A};  // Random data.
    uint16_t expected_crc = 0x48D0;  // Pre-calculated CRC for this input.

    uint16_t crc = modbus_calculate_crc(cmd, sizeof(cmd));

    TEST_ASSERT_EQUAL_HEX16(expected_crc, crc);
}

// Test CRC with maximum data length (255 bytes)
void test_modbus_calculate_crc_max_length(void) {
    uint8_t cmd[255] = {0x01};  // Fill array with test data.
    for (int i = 1; i < 255; i++) {
        cmd[i] = (uint8_t)i;
    }
    uint16_t expected_crc = 0xAD87;  // Pre-calculated CRC for this input.

    uint16_t crc = modbus_calculate_crc(cmd, sizeof(cmd));

    TEST_ASSERT_EQUAL_HEX16(expected_crc, crc);
}

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_modbus_calculate_crc_single_byte);
    RUN_TEST(test_modbus_calculate_crc_two_bytes);
    RUN_TEST(test_modbus_calculate_crc_eight_bytes);
    RUN_TEST(test_modbus_calculate_crc_sixteen_bytes);
    RUN_TEST(test_modbus_calculate_crc_empty_buffer);
    RUN_TEST(test_modbus_calculate_crc_random_data);
    RUN_TEST(test_modbus_calculate_crc_max_length);

    return UNITY_END();
}
