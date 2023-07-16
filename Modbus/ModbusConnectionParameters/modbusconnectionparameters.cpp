#include "ModbusConnectionParameters.hpp"

void ModbusConnectionParameters::SetSerialPort(const QString& serialPort)
{
    _serialPort = serialPort;
}

QString ModbusConnectionParameters::GetSerialPort() const
{
    return _serialPort;
}

void ModbusConnectionParameters::SetBaudRate(qint32 baudRate)
{
    _baudRate = baudRate;
}

qint32 ModbusConnectionParameters::GetBaudRate() const
{
    return _baudRate;
}

void ModbusConnectionParameters::SetDataBits(QSerialPort::DataBits dataBits)
{
    _dataBits = dataBits;
}

QSerialPort::DataBits ModbusConnectionParameters::GetDataBits() const
{
    return _dataBits;
}

void ModbusConnectionParameters::SetParity(QSerialPort::Parity parity)
{
    _parity = parity;
}

QSerialPort::Parity ModbusConnectionParameters::GetParity() const
{
    return _parity;
}

void ModbusConnectionParameters::SetStopBits(QSerialPort::StopBits stopBits)
{
    _stopBits = stopBits;
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

void ModbusConnectionParameters::SetPort(quint16 port)
{
    _port = port;
}

quint16 ModbusConnectionParameters::GetPort() const
{
    return _port;
}
