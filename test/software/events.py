
import logging
from ebs.modbus.device import ModbusDevice
from ebs.modbus.hotplug import ModbusHotplugMixin
from ebs.modbus.events import ModbusEventLogMixin


logging.basicConfig()
log = logging.getLogger()
log.setLevel(logging.INFO)


class TestDevice(ModbusHotplugMixin,
                 ModbusEventLogMixin,
                 ModbusDevice):

    def __init__(self, *args, **kwargs):
        super(TestDevice, self).__init__(*args, **kwargs)

    def print_events(self):
        l = len(self.modbus_events)
        for idx, event in enumerate(self.modbus_events):
            print("{0:5} {1}".format(idx-l, event))


if __name__ == '__main__':
    device = TestDevice(5, method='rtu', port='/dev/ttyACM1')
    device.connect()
    device.print_events()
