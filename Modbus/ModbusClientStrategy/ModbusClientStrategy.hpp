#pragma once
#include <QObject>
#include <ModbusStrategy.hpp>
#include <QModbusClient>
#include <memory>

class ModbusClientStrategy : public ModbusStrategy
{
    Q_OBJECT
public:
    ModbusClientStrategy();
    virtual ~ModbusClientStrategy();

protected:
    std::unique_ptr<QModbusClient> _modbusClient;
};

