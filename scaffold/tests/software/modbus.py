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
import minimalmodbus


SLAVE_ADDRESS = 5
SLAVE_BAUDRATE = 256000
SLAVE_NREGS = 200


@pytest.fixture
def instrument():
    i = minimalmodbus.Instrument('/dev/ttyACM1', SLAVE_ADDRESS)
    i.serial.baudrate = SLAVE_BAUDRATE
    i.serial.timeout = 0.1
    i.debug = True
    return i
