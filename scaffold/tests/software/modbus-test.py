
import minimalmodbus


instrument = minimalmodbus.Instrument('/dev/ttyACM1', 5)
instrument.serial.baudrate = 256000
instrument.serial.timeout = 1
# register_value = instrument.read_register(0)
# print register_value
instrument.write_register(5, 100)
instrument.write_register(6, 1004)
instrument.write_register(7, 04)

print instrument.read_register(5)
print instrument.read_register(6)
print instrument.read_registers(5, 3)
