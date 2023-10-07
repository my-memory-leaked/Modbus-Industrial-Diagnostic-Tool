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

    bool readRegister(int slaveAddress, int registerAddress, quint16 &value);
    bool writeRegister(int slaveAddress, int registerAddress, quint16 value);

public slots:
    void Start();
    void CloseConnection();

private:
    ModbusStrategy *_modbusStrategy;
};
