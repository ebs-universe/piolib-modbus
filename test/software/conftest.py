

def pytest_addoption(parser):
    parser.addoption("--if", help="Interface(s) to use", default=[], action='append')
    parser.addoption("--saddress", help="MODBUS slave address.",
                     default=5, action='store')


def pytest_generate_tests(metafunc):
    if 'client' in metafunc.fixturenames:
        interfaces = metafunc.config.getoption('if')
        ifs = []
        for interface in interfaces:
            try:
                port, baud = interface.split(':')
                baud = int(baud)
            except ValueError:
                port = interface
                baud = 256000
            ifs.append({'port': port, 'baudrate': baud})
        metafunc.parametrize("client", ifs, indirect=True)
