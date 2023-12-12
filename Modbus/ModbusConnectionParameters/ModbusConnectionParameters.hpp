#pragma once

#include <QString>
#include <QtSerialPort/QSerialPort>

/* Class contains both RS485 and TCP IP parameters */
class ModbusConnectionParameters
{
public:
    ModbusConnectionParameters();
    ModbusConnectionParameters(const QString& cIpAddress, const quint16& cPort);

    void SetDefaultValues();

    void SetSerialPort(const QString& serialPort);
    QString GetSerialPort() const;

    void SetBaudRate(const quint32& cBaudRate);
    quint32 GetBaudRate() const;

    void SetDataBits(const QSerialPort::DataBits& cDataBits);
    QSerialPort::DataBits GetDataBits() const;

    void SetParity(const QSerialPort::Parity& cParity);
    QSerialPort::Parity GetParity() const;

    void SetStopBits(const QSerialPort::StopBits& cStopBits);
    QSerialPort::StopBits GetStopBits() const;

    void SetIpAddress(const QString& cIpAddress);
    QString GetIpAddress() const;

    void SetPort(const quint16& cPort);
    quint16 GetPort() const;

private:
    QString _serialPort;
    quint32 _baudRate;
    QSerialPort::DataBits _dataBits;
    QSerialPort::Parity _parity;
    QSerialPort::StopBits _stopBits;
    QString _ipAddress;
    quint16 _port;
};
