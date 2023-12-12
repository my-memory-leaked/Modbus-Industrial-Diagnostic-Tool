#include "ModbusConnectionParameters.hpp"

ModbusConnectionParameters::ModbusConnectionParameters()
{
    SetDefaultValues();
}

ModbusConnectionParameters::ModbusConnectionParameters(const QString& cIpAddress, const quint16& cPort) : _ipAddress(cIpAddress), _port(cPort)
{

}

void ModbusConnectionParameters::SetDefaultValues()
{
    _serialPort = "COM0";
    _baudRate = 115200;

    _dataBits = QSerialPort::Data8;
    _parity = QSerialPort::NoParity;
    _stopBits = QSerialPort::OneStop;

    _ipAddress = "127.0.0.1"; /* Localhost */
    _port = 502;
}



void ModbusConnectionParameters::SetSerialPort(const QString& cSerialPort)
{
    _serialPort = cSerialPort;
}

QString ModbusConnectionParameters::GetSerialPort() const
{
    return _serialPort;
}

void ModbusConnectionParameters::SetBaudRate(const quint32& cBaudRate)
{
    _baudRate = cBaudRate;
}

quint32 ModbusConnectionParameters::GetBaudRate() const
{
    return _baudRate;
}

void ModbusConnectionParameters::SetDataBits(const QSerialPort::DataBits& cDataBits)
{
    _dataBits = cDataBits;
}

QSerialPort::DataBits ModbusConnectionParameters::GetDataBits() const
{
    return _dataBits;
}

void ModbusConnectionParameters::SetParity(const QSerialPort::Parity& cParity)
{
    _parity = cParity;
}

QSerialPort::Parity ModbusConnectionParameters::GetParity() const
{
    return _parity;
}

void ModbusConnectionParameters::SetStopBits(const QSerialPort::StopBits& cStopBits)
{
    _stopBits = cStopBits;
}

QSerialPort::StopBits ModbusConnectionParameters::GetStopBits() const
{
    return _stopBits;
}

void ModbusConnectionParameters::SetIpAddress(const QString& ipAddress)
{
    _ipAddress = ipAddress;
}

QString ModbusConnectionParameters::GetIpAddress() const
{
    return _ipAddress;
}

void ModbusConnectionParameters::SetPort(const quint16& cPort)
{
    _port = cPort;
}

quint16 ModbusConnectionParameters::GetPort() const
{
    return _port;
}
