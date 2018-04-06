
import struct

from pymodbus.constants import Defaults
from pymodbus.client.sync import ModbusSerialClient
from pymodbus.client.sync import BaseModbusClient
from pymodbus.transaction import ModbusAsciiFramer
from pymodbus.transaction import ModbusRtuFramer
from pymodbus.transaction import ModbusBinaryFramer
from pymodbus.transaction import ModbusSocketFramer
from pymodbus.exceptions import ParameterException

from pymodbus.pdu import ExceptionResponse
from pymodbus.pdu import ModbusExceptions as ecode
from pymodbus.interfaces import IModbusDecoder
from pymodbus.exceptions import ModbusException
from pymodbus.bit_read_message import *
from pymodbus.bit_write_message import *
from pymodbus.diag_message import *
from pymodbus.file_message import *
from pymodbus.other_message import *
from pymodbus.mei_message import *
from pymodbus.register_read_message import *
from pymodbus.register_write_message import *
from pymodbus.compat import byte2int
from pymodbus.factory import _logger


class ModbusServerException(Exception):
    def __init_(self, response):
        self.response = response


class ReadEBSDeviceInformationResponse(ReadDeviceInformationResponse):
    def decode(self, data):
        params = struct.unpack('>BBBBBB', data[0:6])
        self.sub_function_code, self.read_code = params[0:2]
        self.conformity, self.more_follows = params[2:4]
        self.next_object_id, self.number_of_objects = params[4:6]
        self.information, count = {}, 6  # skip the header information

        while count < len(data):
            object_id, object_length = struct.unpack('>BB', data[count:count+2])
            count += object_length + 2
            if object_id not in self.information.keys():
                self.information[object_id] = data[count-object_length:count]
            else:
                if isinstance(self.information[object_id], list):
                    self.information[object_id].append(data[count-object_length:count])
                else:
                    self.information[object_id] = [self.information[object_id],
                                                   data[count - object_length:count]]

    def __str__(self):
        return "ReadEBSDeviceInformationResponse(%d)" % self.read_code


# The __ does not allow simple subclassing
class EBSClientDecoder(IModbusDecoder):
    __function_table = [
            ReadHoldingRegistersResponse,
            ReadDiscreteInputsResponse,
            ReadInputRegistersResponse,
            ReadCoilsResponse,
            WriteMultipleCoilsResponse,
            WriteMultipleRegistersResponse,
            WriteSingleRegisterResponse,
            WriteSingleCoilResponse,
            ReadWriteMultipleRegistersResponse,

            DiagnosticStatusResponse,

            ReadExceptionStatusResponse,
            GetCommEventCounterResponse,
            GetCommEventLogResponse,
            ReportSlaveIdResponse,

            ReadFileRecordResponse,
            WriteFileRecordResponse,
            MaskWriteRegisterResponse,
            ReadFifoQueueResponse,

            ReadEBSDeviceInformationResponse,
    ]
    __sub_function_table = [
            ReturnQueryDataResponse,
            RestartCommunicationsOptionResponse,
            ReturnDiagnosticRegisterResponse,
            ChangeAsciiInputDelimiterResponse,
            ForceListenOnlyModeResponse,
            ClearCountersResponse,
            ReturnBusMessageCountResponse,
            ReturnBusCommunicationErrorCountResponse,
            ReturnBusExceptionErrorCountResponse,
            ReturnSlaveMessageCountResponse,
            ReturnSlaveNoReponseCountResponse,
            ReturnSlaveNAKCountResponse,
            ReturnSlaveBusyCountResponse,
            ReturnSlaveBusCharacterOverrunCountResponse,
            ReturnIopOverrunCountResponse,
            ClearOverrunCountResponse,
            GetClearModbusPlusResponse,

            ReadEBSDeviceInformationResponse,
    ]

    def __init__(self):
        functions = set(f.function_code for f in self.__function_table)
        self.__lookup = dict([(f.function_code, f) for f in self.__function_table])
        self.__sub_lookup = dict((f, {}) for f in functions)
        for f in self.__sub_function_table:
            self.__sub_lookup[f.function_code][f.sub_function_code] = f

    def lookupPduClass(self, function_code):
        return self.__lookup.get(function_code, ExceptionResponse)

    def decode(self, message):
        try:
            return self._helper(message)
        except ModbusException as er:
            _logger.error("Unable to decode response %s" % er)
        return None

    def _helper(self, data):
        function_code = byte2int(data[0])
        _logger.debug("Factory Response[%d]" % function_code)
        response = self.__lookup.get(function_code, lambda: None)()
        if function_code > 0x80:
            code = function_code & 0x7f  # strip error portion
            response = ExceptionResponse(code, ecode.IllegalFunction)
        if not response:
            raise ModbusException("Unknown response %d" % function_code)
        response.decode(data[1:])

        if hasattr(response, 'sub_function_code'):
            lookup = self.__sub_lookup.get(response.function_code, {})
            subtype = lookup.get(response.sub_function_code, None)
            if subtype:
                response.__class__ = subtype
        return response


# The staticmethod on implementation() does not allow simple subclassing
class ModbusClient(ModbusSerialClient):
    def __init__(self, method='ascii', **kwargs):
        self.method = method
        self.socket = None
        BaseModbusClient.__init__(self, self._get_framer(method), **kwargs)

        self.port     = kwargs.get('port', 0)
        self.stopbits = kwargs.get('stopbits', Defaults.Stopbits)
        self.bytesize = kwargs.get('bytesize', Defaults.Bytesize)
        self.parity   = kwargs.get('parity',   Defaults.Parity)
        self.baudrate = kwargs.get('baudrate', Defaults.Baudrate)
        self.timeout  = kwargs.get('timeout',  Defaults.Timeout)
        if self.method == "rtu":
            self._last_frame_end = 0.0
            self._silent_interval = 3.5 * (1 + 8 + 2) / self.baudrate

    def execute(self, request=None):
        result = super(ModbusClient, self).execute(request)
        if isinstance(result, ExceptionResponse):
            raise ModbusServerException(result)
        return result

    def _get_framer(self, method):
        method = method.lower()
        if method == 'ascii':
            return ModbusAsciiFramer(EBSClientDecoder())
        elif method == 'rtu':
            return ModbusRtuFramer(EBSClientDecoder())
        elif method == 'binary':
            return ModbusBinaryFramer(EBSClientDecoder())
        elif method == 'socket':
            return ModbusSocketFramer(EBSClientDecoder())
        raise ParameterException("Invalid framer method requested")
