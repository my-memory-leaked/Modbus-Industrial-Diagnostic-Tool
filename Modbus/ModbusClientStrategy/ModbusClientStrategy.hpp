#pragma once
#include <ModbusStrategy.hpp>
#include <QModbusClient>
#include <memory>

class ModbusClientStrategy : public ModbusStrategy
{
public:
    ModbusClientStrategy();
    ~ModbusClientStrategy();

protected:
    std::unique_ptr<QModbusClient> _modbusClient;
};

