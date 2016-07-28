

def pytest_addoption(parser):
    parser.addoption("--port", help="Serial port to use.",
                     default='ttyACM1', action='store')
    parser.addoption("--baud", help="Serial port baud rate to use.",
                     default=256000, action='store')
    parser.addoption("--saddress", help="MODBUS slave address.",
                     default=5, action='store')
