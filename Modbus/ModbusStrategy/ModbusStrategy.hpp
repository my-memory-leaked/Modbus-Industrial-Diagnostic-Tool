#pragma once

#include <QObject>
#include <QModbusDataUnit>
#include <SystemResult.hpp>
#include <ModbusConnectionParameters.hpp>
#include <DeviceInterface.hpp>

class ModbusStrategy : public QObject, public DeviceInterface
{
    Q_OBJECT
public:
    ModbusStrategy(QObject *parent = nullptr);
    virtual ~ModbusStrategy();

    virtual SystemResult SetConnectionParameters(const ModbusConnectionParameters &cConnectionParameters) = 0;

    virtual SystemResult ReadData(const QModbusDataUnit &cData) = 0;
    virtual SystemResult WriteData(const QModbusDataUnit &cData) = 0;
protected:
    ModbusConnectionParameters _connectionParameters;

};
