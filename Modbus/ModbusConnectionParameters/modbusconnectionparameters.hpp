#pragma once

#include <QString>
#include <QtSerialPort/QSerialPort>

class ModbusConnectionParameters
{
public:
    void SetSerialPort(const QString& serialPort);
    QString GetSerialPort() const;

    void SetBaudRate(qint32 baudRate);
    qint32 GetBaudRate() const;

    void SetDataBits(QSerialPort::DataBits dataBits);
    QSerialPort::DataBits GetDataBits() const;

    void SetParity(QSerialPort::Parity parity);
    QSerialPort::Parity GetParity() const;

    void SetStopBits(QSerialPort::StopBits stopBits);
    QSerialPort::StopBits GetStopBits() const;

    void SetIpAddress(const QString& ipAddress);
    QString GetIpAddress() const;

    void SetPort(quint16 port);
    quint16 GetPort() const;

private:
    QString _serialPort;
    qint32 _baudRate;
    QSerialPort::DataBits _dataBits;
    QSerialPort::Parity _parity;
    QSerialPort::StopBits _stopBits;
    QString _ipAddress;
    quint16 _port;
};
