#pragma once

#include <QModbusDataUnit>
#include <SystemResult.hpp>
#include <ModbusConnectionParameters.hpp>
#include <DeviceInterface.hpp>

class ModbusStrategy : public DeviceInterface
{
public:
    ModbusStrategy();
    virtual ~ModbusStrategy();

    virtual SystemResult SetConnectionParameters(const ModbusConnectionParameters &cConnectionParameters);

    virtual SystemResult ReadData(const QModbusDataUnit &cData) = 0;
    virtual SystemResult WriteData(const QModbusDataUnit &cData) = 0;
protected:
    ModbusConnectionParameters _connectionParameters;

};
