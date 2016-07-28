
import pytest
from modbus import instrument
from modbus import SLAVE_NREGS


def test_modbus_rdregs_preliminary(instrument):
    rval = instrument.read_register(0)
    assert rval == instrument.address


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


def test_wrmregs_2b(instrument):
    instrument.write_registers(7, [322, 12, 4000])
    rval = instrument.read_register(7)
    assert rval == 322
    rval = instrument.read_register(8)
    assert rval == 12
    rval = instrument.read_register(9)
    assert rval == 4000


def test_wrmregs_protected(instrument):
    pass


def test_wrmregs_outofrange_start(instrument):
    with pytest.raises(ValueError):
        instrument.write_registers(247, [100, 100, 100])
    with pytest.raises(ValueError):
        instrument.write_registers(259, [100, 100, 100])


def test_wrmregs_outofrange_end(instrument):
    with pytest.raises(ValueError):
        instrument.write_registers(63, [100, 100, 100])


def test_rdregs_hr_1r_1b(instrument):
    rval = instrument.read_register(5, functioncode=3)
    assert rval == 100


def test_rdregs_ir_1r_1b(instrument):
    rval = instrument.read_register(5, functioncode=4)
    assert rval == 100


def test_rdregs_ir_1r_2b(instrument):
    rval = instrument.read_register(6, functioncode=4)
    assert rval == 1004


def test_rdregs_ir_mulr_1b(instrument):
    rval = instrument.read_registers(5, 5, functioncode=4)
    assert rval == [100, 1004, 322, 12, 4000]


def test_rdregs_ir_mulr_2b(instrument):
    pass


def test_rdregs_outofrange_start(instrument):
    with pytest.raises(ValueError):
        instrument.read_registers(247, 3)
    with pytest.raises(ValueError):
        instrument.read_registers(259, 3)


def test_rdregs_outofrange_end(instrument):
    with pytest.raises(ValueError):
        instrument.read_registers(63, 3)


def test_wrregm(instrument):
    pass


def test_rwmregs(instrument):
    pass
