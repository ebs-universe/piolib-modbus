
import pytest
from modbus import client, saddress
from modbus import SLAVE_NREGS

from pymodbus.exceptions import ModbusIOException
from pymodbus import diag_message
from pymodbus import other_message


def test_rexcst(client, saddress):
    request = other_message.ReadExceptionStatusRequest(unit=saddress)
    response = client.execute(request)
    assert response.status == 0x80


def test_rexcst_broadcast(client):
    request = other_message.ReadExceptionStatusRequest(unit=0x00)
    response = client.execute(request)
    assert isinstance(response, ModbusIOException)
