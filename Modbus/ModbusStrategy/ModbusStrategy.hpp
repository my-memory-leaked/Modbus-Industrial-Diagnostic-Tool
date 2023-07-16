#pragma once

#include <QObject>
#include <QModbusDataUnit>
#include "ModbusConnectionParameters.hpp"

class ModbusStrategy : public QObject
{
    Q_OBJECT

public:
    explicit ModbusStrategy(const ModbusConnectionParameters &cConnectionParams, QObject *parent = nullptr);
    virtual ~ModbusStrategy() {}

    virtual void Start() = 0;
    virtual void SendData(const QModbusDataUnit &cData) = 0;
    virtual void CloseConnection() = 0;
protected:
    ModbusConnectionParameters _connectionParams;
};
