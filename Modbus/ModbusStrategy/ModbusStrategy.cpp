#include <ModbusStrategy.hpp>

ModbusStrategy::ModbusStrategy() {}

ModbusStrategy::~ModbusStrategy() {}

SystemResult ModbusStrategy::SetConnectionParameters(const ModbusConnectionParameters &cConnectionParameters)
{
    _connectionParameters = cConnectionParameters;
    return SystemResult::SYSTEM_OK;
}

ModbusConnectionParameters ModbusStrategy::GetConnectionParameters() const
{
    return _connectionParameters;
}

QModbusDevice::State ModbusStrategy::GetState() const
{
    return _state;
}

ModbusStrategy::ModbusInterfaceType ModbusStrategy::GetInterfaceType() const
{
    return _type;
}

void ModbusStrategy::emitModbusStateUpdated(const QModbusDevice::State& cState)
{
    emit ModbusStateUpdated(_deviceName, cState);
}
