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

import pytest

try:
    from ebs.modbus.client import ModbusClient
    from ebs.modbus.client import ModbusServerException

except ImportError:
    ModbusServerException = None
    from pymodbus.client.sync import ModbusSerialClient as ModbusClient

SLAVE_NREGS = 64
TEST_ADDRESS_BASE = 14


@pytest.fixture
def client(request):
    mclient = ModbusClient(method='rtu', timeout=0.1, **request.param)
    mclient.connect()

    # Claim interface via hotplug
    try:
        mclient.write_register(1, 1, unit=5)
    except ModbusServerException:
        pass

    # Clear test field
    mclient.write_register(TEST_ADDRESS_BASE, 0x00, unit=5)

    def done():
        mclient.close()
    request.addfinalizer(done)
    return mclient


@pytest.fixture
def saddress(request):
    return int(request.config.getoption('--saddress'))
