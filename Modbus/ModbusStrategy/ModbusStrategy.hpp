#pragma once

#include <QObject>
#include <DeviceInterface.hpp>
#include <SystemResult.hpp>
#include <QModbusDataUnit>
#include <ModbusConnectionParameters.hpp>
class ModbusStrategy : public DeviceInterface
{
    Q_OBJECT
public:
    ModbusStrategy();
    virtual ~ModbusStrategy();

    virtual SystemResult SetConnectionParameters(const ModbusConnectionParameters &cConnectionParameters);
    virtual ModbusConnectionParameters GetConnectionParameters() const;

    virtual SystemResult ReadData(const QModbusDataUnit &cData) = 0;
    virtual SystemResult WriteData(const QModbusDataUnit &cData) = 0;
protected:
    ModbusConnectionParameters _connectionParameters;
};
