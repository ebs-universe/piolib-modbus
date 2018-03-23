#!/usr/bin/env python
# encoding: utf-8

# Copyright (C) 2016 Chintalagiri Shashank
#
# This file is part of modbus-test.
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU Affero General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Affero General Public License for more details.
#
# You should have received a copy of the GNU Affero General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

"""
Docstring for modbus
"""

import os
import pytest
from pymodbus.client.sync import ModbusSerialClient
from pymodbus.pdu import ExceptionResponse

SLAVE_NREGS = 200


class ModbusServerException(Exception):
    def __init_(self, response):
        self.response = response


class ModbusClient(ModbusSerialClient):
    def execute(self, request=None):
        result = super(ModbusClient, self).execute(request)
        if isinstance(result, ExceptionResponse):
            raise ModbusServerException(result)
        return result


@pytest.fixture
def client(request):
    port = os.path.join('/dev', request.config.getoption('--port'))
    mclient = ModbusClient(method='rtu', port=port, timeout=0.1,
                           baudrate=int(request.config.getoption('--baud')))
    mclient.connect()

    def done():
        mclient.close()
    request.addfinalizer(done)
    return mclient


@pytest.fixture
def saddress(request):
    return int(request.config.getoption('--saddress'))
