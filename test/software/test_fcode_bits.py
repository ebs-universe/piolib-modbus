
import pytest
from modbus import client, saddress
from modbus import SLAVE_NREGS, TEST_ADDRESS_BASE
from modbus import ModbusServerException

from pymodbus import bit_write_message
from pymodbus import bit_read_message


BIT_ADDRESS_BASE = (TEST_ADDRESS_BASE) * 16


def test_wrsbit_set(client, saddress):
    request = bit_write_message.WriteSingleCoilRequest(
        BIT_ADDRESS_BASE + 32, 1, unit=5
    )
    response = client.execute(request)
    rval = client.read_holding_registers(TEST_ADDRESS_BASE + 2, unit=saddress)
    assert rval.registers[0] == 0x0001


def test_wrsbit_set_ne(client, saddress):
    request = bit_write_message.WriteSingleCoilRequest(
        BIT_ADDRESS_BASE + 32, 1, unit=5
    )
    client.execute(request)
    rval = client.read_holding_registers(TEST_ADDRESS_BASE + 2, unit=saddress)
    assert rval.registers[0] == 0x0001


def test_wrsbit_clear(client, saddress):
    request = bit_write_message.WriteSingleCoilRequest(
        BIT_ADDRESS_BASE + 32, 0, unit=5
    )
    client.execute(request)
    rval = client.read_holding_registers(TEST_ADDRESS_BASE + 2, unit=saddress)
    assert rval.registers[0] == 0x0000


def test_wrsbit_wp(client, saddress):
    request = bit_write_message.WriteSingleCoilRequest(
        5 * 16, 1, unit=5
    )
    with pytest.raises(ModbusServerException):
        client.execute(request)
    rval = client.read_holding_registers(6, unit=saddress)
    assert rval.registers[0] == 0x0000
