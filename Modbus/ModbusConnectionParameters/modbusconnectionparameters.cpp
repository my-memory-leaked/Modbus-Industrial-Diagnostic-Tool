#include "ModbusConnectionParameters.hpp"

void ModbusConnectionParameters::SetSerialPort(const QString& serialPort)
{
    _serialPort = serialPort;
}

QString ModbusConnectionParameters::GetSerialPort() const
{
    return _serialPort;
}

void ModbusConnectionParameters::SetBaudRate(const quint32 cBaudRate)
{
    _baudRate = cBaudRate;
}

quint32 ModbusConnectionParameters::GetBaudRate() const
{
    return _baudRate;
}

void ModbusConnectionParameters::SetDataBits(const QSerialPort::DataBits cDataBits)
{
    _dataBits = cDataBits;
}

QSerialPort::DataBits ModbusConnectionParameters::GetDataBits() const
{
    return _dataBits;
}

void ModbusConnectionParameters::SetParity(const QSerialPort::Parity cParity)
{
    _parity = _parity;
}

QSerialPort::Parity ModbusConnectionParameters::GetParity() const
{
    return _parity;
}

void ModbusConnectionParameters::SetStopBits(const QSerialPort::StopBits cStopBits)
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

void ModbusConnectionParameters::SetPort(const quint16 cPort)
{
    _port = cPort;
}

quint16 ModbusConnectionParameters::GetPort() const
{
    return _port;
}
