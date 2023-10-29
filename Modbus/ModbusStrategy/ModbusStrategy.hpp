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
    enum class ModbusInterfaceType
    {
        TCP,
        RTU,
        ASCII
    };
    ModbusStrategy();
    virtual ~ModbusStrategy();

    virtual SystemResult SetConnectionParameters(const ModbusConnectionParameters &cConnectionParameters);
    virtual ModbusConnectionParameters GetConnectionParameters() const;

    virtual QModbusDevice::State GetState() const;
    virtual ModbusInterfaceType GetInterfaceType() const;

    virtual QModbusReply *ReadData(const QModbusDataUnit &cData) = 0;
    virtual QModbusReply *WriteData(const QModbusDataUnit &cData) = 0;

signals:
    void ModbusStateUpdated(const QString& cDeviceName, const QModbusDevice::State& cState);
protected:
    ModbusConnectionParameters _connectionParameters;
    QModbusDevice::State _state;
    ModbusInterfaceType _type;

    virtual void emitModbusStateUpdated(const QModbusDevice::State& cState);
};
