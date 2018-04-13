
import random

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


def test_diag_rqdata(client, saddress):
    message = random.randint(0, 2**16)
    request = diag_message.ReturnQueryDataRequest(message=message, unit=saddress)
    response = client.execute(request)
    assert response.message == message


def test_diag_rqdata_broadcast(client):
    request = diag_message.ReturnQueryDataRequest(message=0x7762)
    response = client.execute(request)
    assert isinstance(response, ModbusIOException)


def test_diag_forcelistenonly(client, saddress):
    # TODO Check that no actions are taken when silenced
    # TODO Check other side effects of Restart Communications Option Request
    request = diag_message.ForceListenOnlyModeRequest(unit=saddress)
    response = client.execute(request)
    assert isinstance(response, ModbusIOException)

    message = random.randint(0, 2 ** 16)
    request = diag_message.ReturnQueryDataRequest(message=message, unit=saddress)
    response = client.execute(request)
    assert isinstance(response, ModbusIOException)

    request = diag_message.RestartCommunicationsOptionRequest(unit=saddress)
    response = client.execute(request)
    assert isinstance(response, ModbusIOException)

    message = random.randint(0, 2 ** 16)
    request = diag_message.ReturnQueryDataRequest(message=message, unit=saddress)
    response = client.execute(request)
    assert response.message == message

    request = diag_message.RestartCommunicationsOptionRequest(unit=saddress)
    response = client.execute(request)
    assert isinstance(response, diag_message.RestartCommunicationsOptionResponse)
