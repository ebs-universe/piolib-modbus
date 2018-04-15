import setuptools

setuptools.setup(
    name="ebs-modbus",
    url="https://github.com/chintal/ebs-lib-modbus",

    author="Chintalagiri Shashank",
    author_email="shashank.chintalagiri@gmail.com",

    description="Python host interface to EBS modbus devices",
    long_description=open('README.rst').read(),

    packages=setuptools.find_packages(),

    install_requires=['pymodbus'],

    classifiers=[
        'Development Status :: 2 - Pre-Alpha',
        'Programming Language :: Python',
        'Programming Language :: Python :: 2',
        'Programming Language :: Python :: 2.7',
        'Programming Language :: Python :: 3',
        'Programming Language :: Python :: 3.6',
    ],
)
