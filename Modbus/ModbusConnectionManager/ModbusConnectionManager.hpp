#pragma once

#include <QThread>
#include <QModbusClient>
#include "ModbusStrategy.hpp"


class ModbusConnectionManager : public QObject
{
    Q_OBJECT

public:
    explicit ModbusConnectionManager(ModbusStrategy *modbusStrategy, QObject *parent = nullptr);
    ~ModbusConnectionManager();

public slots:
    void Start();
    void SendData(const QModbusDataUnit &cData);
    void Disconnect();

private:
    ModbusStrategy *_modbusStrategy;
};
