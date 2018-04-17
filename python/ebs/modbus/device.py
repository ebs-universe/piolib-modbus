

from .client import ModbusClient
from functools import partial
from pprint import PrettyPrinter


class ModbusDevice(object):
    _client_functions = [
        'read_coils',
        'read_discrete_inputs',
        'write_coil',
        'write_coils',
        'write_register',
        'write_registers',
        'read_holding_registers',
        'read_input_registers',
        'readwrite_registers',
        'mask_write_register',
    ]

    def __init__(self, address, mc=None, **kwargs):
        self.address = address
        if mc is None:
            kwargs.setdefault('method', 'rtu')
            kwargs.setdefault('port', '/dev/ttyACM1')
            kwargs.setdefault('baudrate', 256000)
            kwargs.setdefault('timeout', 0.1)
            mc = ModbusClient(**kwargs)
        self.mc = mc
        self._registry = {
            'modbus_client_functions': self._client_functions,
        }
        super(ModbusDevice, self).__init__()

    def connect(self):
        return self.mc.connect()

    def close(self):
        return self.mc.close()

    def execute(self, request, broadcast=False):
        if not broadcast:
            request.unit_id = self.address
        else:
            request.unit_id = 0x00
        return self.mc.execute(request)

    def print_registry(self):
        pp = PrettyPrinter(indent=4)
        pp.pprint(self._registry)

    def __getattr__(self, name):
        if name in self._registry['modbus_client_functions']:
            return partial(getattr(self.mc, name), unit=self.address)
        raise AttributeError('{0} object has not attribute {1}'
                             ''.format(self.__class__, name))
