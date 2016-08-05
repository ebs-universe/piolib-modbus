
import pytest
from modbus import client, saddress
from modbus import SLAVE_NREGS

from pymodbus import register_write_message
from pymodbus import register_read_message


TEST_ADDRESS_BASE = 20


def test_modbus_rdregs_preliminary(client, saddress):
    rval = client.read_holding_registers(0, unit=saddress)
    assert rval.registers[0] == saddress


def test_modbus_broadcast_rdregs_preliminary(client, saddress):
    rval = client.read_holding_registers(0, unit=0x00)
    assert rval is None


def test_wrsreg_1b(client, saddress):
    request = register_write_message.WriteSingleRegisterRequest(
        TEST_ADDRESS_BASE, 100, unit=5
    )
    response = client.execute(request)
    rval = client.read_holding_registers(TEST_ADDRESS_BASE, unit=saddress)
    assert rval.registers[0] == 100


def test_wrsreg_broadcast(client, saddress):
    request = register_write_message.WriteSingleRegisterRequest(
        TEST_ADDRESS_BASE + 10, 100, unit=0x00)
    response = client.execute(request)
    rval = client.read_holding_registers(TEST_ADDRESS_BASE + 10, unit=saddress)
    assert rval.registers[0] == 100


def test_wrsreg_2b(client, saddress):
    request = register_write_message.WriteSingleRegisterRequest(
        TEST_ADDRESS_BASE + 1, 1004, unit=5
    )
    response = client.execute(request)
    rval = client.read_holding_registers(TEST_ADDRESS_BASE + 1, unit=saddress)
    assert rval.registers[0] == 1004


def test_wrsreg_protected(client, saddress):
    request = register_write_message.WriteSingleRegisterRequest(
        1, 4, unit=saddress)
    response = client.execute(request)
    assert response.function_code > 0x80
    rval = client.read_holding_registers(6, unit=saddress)
    assert rval != 4


def test_wrsreg_outofrange_1b(client, saddress):
    request = register_write_message.WriteSingleRegisterRequest(
        SLAVE_NREGS + 1, 04, unit=saddress)
    response = client.execute(request)
    assert response.function_code > 0x80


def test_wrsreg_outofrange_2b(client, saddress):
    request = register_write_message.WriteSingleRegisterRequest(
        266, 04, unit=saddress)
    response = client.execute(request)
    assert response.function_code > 0x80


def test_wrmregs_2b(client, saddress):
    request = register_write_message.WriteMultipleRegistersRequest(
        address=TEST_ADDRESS_BASE + 2, values=[322, 12, 4000], unit=saddress
    )
    response = client.execute(request)
    rval = client.read_holding_registers(TEST_ADDRESS_BASE + 2, unit=saddress)
    assert rval.registers[0] == 322
    rval = client.read_holding_registers(TEST_ADDRESS_BASE + 3, unit=saddress)
    assert rval.registers[0] == 12
    rval = client.read_holding_registers(TEST_ADDRESS_BASE + 4, unit=saddress)
    assert rval.registers[0] == 4000


def test_wrmregs_protected(client, saddress):
    pass


def test_wrmregs_outofrange_start(client, saddress):
    request = register_write_message.WriteMultipleRegistersRequest(
        address=247, values=[100, 100, 100], unit=saddress)
    response = client.execute(request)
    assert response.function_code > 0x80


def test_wrmregs_outofrange_end(client, saddress):
    request = register_write_message.WriteMultipleRegistersRequest(
        address=63, values=[100, 100, 100], unit=saddress)
    response = client.execute(request)
    assert response.function_code > 0x80


def test_rdregs_hr_1r_1b(client, saddress):
    rval = client.read_holding_registers(TEST_ADDRESS_BASE, unit=saddress)
    assert rval.registers[0] == 100


def test_rdregs_ir_1r_1b(client, saddress):
    rval = client.read_input_registers(TEST_ADDRESS_BASE, unit=saddress)
    assert rval.registers[0] == 100


def test_rdregs_ir_1r_2b(client, saddress):
    rval = client.read_input_registers(TEST_ADDRESS_BASE + 1, unit=saddress)
    assert rval.registers[0] == 1004


def test_rdregs_ir_mulr_1b(client, saddress):
    rval = client.read_input_registers(TEST_ADDRESS_BASE, count=5, unit=saddress)
    assert rval.registers == [100, 1004, 322, 12, 4000]


def test_rdregs_ir_mulr_2b(client, saddress):
    pass


def test_rdregs_outofrange_start(client, saddress):
    rval = client.read_holding_registers(247, count=3, unit=saddress)
    assert rval.function_code > 0x80
    rval = client.read_holding_registers(259, count=3, unit=saddress)
    assert rval.function_code > 0x80


def test_rdregs_outofrange_end(client, saddress):
    rval = client.read_holding_registers(63, count=3, unit=saddress)
    assert rval.function_code > 0x80


def test_wrregm(client, saddress):
    pass


def test_rwmregs(client, saddress):
    pass
