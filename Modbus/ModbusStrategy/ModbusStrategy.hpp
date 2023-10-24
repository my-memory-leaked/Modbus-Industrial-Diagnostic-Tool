#pragma once

#include <QObject>
#include <QModbusDataUnit>
#include <QModbusReply>
#include <DeviceInterface.hpp>
#include <SystemResult.hpp>
#include <ModbusConnectionParameters.hpp>

class ModbusStrategy : public DeviceInterface
{
    Q_OBJECT
public:
    ModbusStrategy();
    virtual ~ModbusStrategy();

    virtual SystemResult SetConnectionParameters(const ModbusConnectionParameters &cConnectionParameters);
    virtual ModbusConnectionParameters GetConnectionParameters() const;

    virtual QModbusReply *ReadData(const QModbusDataUnit &cData) = 0;
    virtual QModbusReply *WriteData(const QModbusDataUnit &cData) = 0;
protected:
    ModbusConnectionParameters _connectionParameters;
};
