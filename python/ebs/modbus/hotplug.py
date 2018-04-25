
from pymodbus.exceptions import ParameterException

from .client import ModbusServerException
from .device import ModbusDevice
from .events import EventParser
from .events import ModbusEventBase

HOTPLUG_EVENT_MASK = 0xF8
HOTPLUG_EVENT_SIG = 0x08


class ModbusHotplugEvent(ModbusEventBase):
    _name = "HOTPLUG"

    def __init__(self):
        self.tag = None

    def decode(self, event):
        ecode = ord(event)
        if (ecode & HOTPLUG_EVENT_MASK) != HOTPLUG_EVENT_SIG:
            raise ParameterException(
                "Event code {0} does not correspond to a hotplug event"
                "".format(ecode)
            )
        self.tag = ecode & (~HOTPLUG_EVENT_MASK & 0xFF)

    def _information_repr(self):
        return "TRANSPORT {0}".format(self.tag)


class ModbusHotplugMixin(object):
    _hotplug_event_parsers = [
        EventParser(HOTPLUG_EVENT_MASK, HOTPLUG_EVENT_SIG, ModbusHotplugEvent)
    ]

    def __init__(self, *args, **kwargs):
        super(ModbusHotplugMixin, self).__init__(*args, **kwargs)
        if not hasattr(self, '_registry'):
            self._registry = {}
        if 'modbus_eventlog_parsers' not in self._registry.keys():
            self._registry['modbus_eventlog_parsers'] = []
        self._registry['modbus_eventlog_parsers'].extend(self._hotplug_event_parsers)

    def claim_interface(
            self  # type: ModbusDevice
    ):
        try:
            self.write_register(1, 1, unit=self.address)
        except ModbusServerException:
            pass

    def connect(
            self  # type: ModbusDevice
    ):
        super(ModbusHotplugMixin, self).connect()
        self.claim_interface()

    def __getattr__(self, item):
        return super(ModbusHotplugMixin, self).__getattr__(item)
