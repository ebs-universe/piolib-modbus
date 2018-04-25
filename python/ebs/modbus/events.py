

from collections import namedtuple
from pymodbus.other_message import GetCommEventLogResponse
from pymodbus.other_message import GetCommEventLogRequest
from pymodbus.events import EnteredListenModeEvent
from pymodbus.events import CommunicationRestartEvent
from pymodbus.events import RemoteReceiveEvent
from pymodbus.events import RemoteSendEvent
from pymodbus.events import ModbusEvent
from pymodbus.compat import int2byte


EventParser = namedtuple('EventParser', 'mask signature eventclass')


def _parse_event(ecode, parsers):
    for parser in parsers:
        if (ecode & parser.mask) == parser.signature:
            event = parser.eventclass()
            event.decode(int2byte(ecode))
            return event
    else:
        return ecode


class ModbusEventRenderMixin(object):
    _name = "UNNAMED EVENT"
    _props = []
    _fl_name = 12
    _fl_property = 8
    _fstring = "{0:" + str(_fl_name) + "}{1}"

    def _information_repr(self):
        _fs = ""
        _props = []
        for idx, prop in enumerate(self._props):
            _fs += "{{{0}:{1}}}".format(idx, self._fl_property)
            _prop = getattr(self, prop[0])
            if _prop:
                _props.append(prop[1].center(self._fl_property, '.'))
            else:
                _props.append('.' * self._fl_property)
        return _fs.format(*_props)

    def __repr__(self):
        return self._fstring.format(self._name.ljust(self._fl_name, '.'),
                                    self._information_repr()).rstrip('.')


class ModbusEventBase(ModbusEvent, ModbusEventRenderMixin):
    pass


class EBSEnteredListenModeEvent(EnteredListenModeEvent, ModbusEventRenderMixin):
    _name = "ENTLISTEN"


class EBSCommunicationRestartEvent(CommunicationRestartEvent, ModbusEventRenderMixin):
    _name = "RESTCOMM"


class EBSRemoteReceiveEvent(RemoteReceiveEvent, ModbusEventRenderMixin):
    _name = "REMOTERECV"
    _props = [('listen', 'LISTEN'),
              ('broadcast', 'BCAST'),
              ('overrun', 'OVERRUN')]


class EBSRemoteSendEvent(RemoteSendEvent, ModbusEventRenderMixin):
    _name = "REMOTESEND"
    _props = [('listen', 'LISTEN'),
              ('read', 'EXCREAD'),
              ('slave_abort', 'EXCABORT'),
              ('slave_busy', 'EXCBUSY'),
              ('slave_nak', 'EXCNAK'),
              ('write_timeout', 'WTIMEOUT')]


class EBSGetCommEventLogResponse(GetCommEventLogResponse):
    _rtu_byte_count_pos = 2

    _modbus_comm_event_parsers = [
        EventParser(0xFF, 0x00, EBSCommunicationRestartEvent),
        EventParser(0xFF, 0x0F, EBSEnteredListenModeEvent),
        EventParser(0x80, 0x80, EBSRemoteReceiveEvent),
        EventParser(0xC0, 0x40, EBSRemoteSendEvent)
    ]

    def parse_event(self, ecode):
        return _parse_event(ecode, self._modbus_comm_event_parsers)

    def decode(self, data):
        super(EBSGetCommEventLogResponse, self).decode(data)
        self.events = [self.parse_event(x) for x in self.events]


class ModbusEventLogMixin(object):
    def __init__(self, *args, **kwargs):
        self._events = []
        super(ModbusEventLogMixin, self).__init__(*args, **kwargs)
        if not hasattr(self, '_registry'):
            self._registry = {}
        if 'modbus_eventlog_parsers' not in self._registry.keys():
            self._registry['modbus_eventlog_parsers'] = []

    @property
    def modbus_events(self):
        if not self._events:
            self.get_modbus_eventlog()
        return self._events

    def get_modbus_eventlog(self):
        self._events = self._get_parsed_modbus_eventlog()

    def _parse_modbus_event(self, event):
        if isinstance(event, int):
            return _parse_event(event,
                                self._registry['modbus_eventlog_parsers'])
        else:
            return event

    def _get_parsed_modbus_eventlog(self):
        events = self._get_raw_modbus_eventlog()
        events = [self._parse_modbus_event(x) for x in events]
        return events

    def _get_raw_modbus_eventlog(self):
        rq = GetCommEventLogRequest(unit=self.address)
        rr = self.mc.execute(rq)
        return rr.events

    def __getattr__(self, item):
        return super(ModbusEventLogMixin, self).__getattr__(item)
