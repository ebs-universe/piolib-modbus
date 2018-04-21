

#include <stdint.h>
#include "modbus.h"


uint16_t modbus_calculate_crc(uint8_t * cmd, uint8_t len)
{
  uint16_t crc = 0xFFFF;
  uint8_t pos, i;
  
  for (pos = 0; pos < len; pos++) {
    crc ^= (uint16_t)cmd[pos];        // XOR byte into least sig. byte of crc
  
    for (i = 8; i != 0; i--) {        // Loop over each bit
      if ((crc & 0x0001) != 0) {      // If the LSB is set
        crc >>= 1;                    // Shift right and XOR 0xA001
        crc ^= 0xA001;
      }
      else                            // Else LSB is not set
        crc >>= 1;                    // Just shift right
    }
    
  }
  return crc;  
}

