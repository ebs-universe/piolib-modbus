
from functools import partial
from pprint import PrettyPrinter

from pymodbus.other_message import ReadExceptionStatusRequest
from .client import ModbusClient


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

    @property
    def exception_status(self):
        return self.read_exception_status()

    def read_exception_status(self):
        request = ReadExceptionStatusRequest(unit=self.address)
        return self.mc.execute(request).status

    def print_registry(self):
        pp = PrettyPrinter(indent=4)
        pp.pprint(self._registry)

    def __getattr__(self, name):
        if name in self._registry['modbus_client_functions']:
            return partial(getattr(self.mc, name), unit=self.address)
        raise AttributeError(
            """{0} object has no attribute {1}. 
            
            Note that due to the method used to dispatch delegated methods 
            and properties, this error could be misleading. If the error 
            resulted from a @property which itself raised an AttributeError, 
            there would have been a lost traceback. Call the underlying 
            non-@property method directly for debugging.
            
            See https://medium.com/@ceshine/python-debugging-pitfall-mixed-use-of-property-and-getattr-f89e0ede13f1
            """.format(self.__class__, name))
