#pragma once
#include <QObject>
#include <ModbusStrategy.hpp>


class ModbusTCPClient : public ModbusStrategy
{
public:
    ModbusTCPClient();
    ~ModbusTCPClient();

    SystemResult SetConnectionParameters(const ModbusConnectionParameters &cConnectionParameters) override;

    SystemResult ReadData(const QModbusDataUnit &cData) override;
    SystemResult WriteData(const QModbusDataUnit &cData) override;
};



