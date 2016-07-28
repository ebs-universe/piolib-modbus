
import pytest
from modbus import instrument
from modbus import SLAVE_ADDRESS
from modbus import SLAVE_NREGS


def test_modbus_rdregs_preliminary(instrument):
    rval = instrument.read_register(0)
    assert rval == SLAVE_ADDRESS


def test_wrsreg_1b(instrument):
    instrument.write_register(5, 100, functioncode=6)
    rval = instrument.read_register(5)
    assert rval == 100


def test_wrsreg_2b(instrument):
    instrument.write_register(6, 1004, functioncode=6)
    rval = instrument.read_register(6)
    assert rval == 1004


def test_wrsreg_protected(instrument):
    with pytest.raises(ValueError):
        instrument.write_register(1, 4, functioncode=6)
    rval = instrument.read_register(1)
    assert rval != 4


def test_wrsreg_outofrange_1b(instrument):
    with pytest.raises(ValueError):
        instrument.write_register(SLAVE_NREGS+1, 04, functioncode=6)


def test_wrsreg_outofrange_2b(instrument):
    with pytest.raises(ValueError):
        instrument.write_register(266, 04, functioncode=6)
