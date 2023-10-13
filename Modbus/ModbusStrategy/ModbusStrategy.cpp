#include <ModbusStrategy.hpp>

ModbusStrategy::ModbusStrategy() {}

ModbusStrategy::~ModbusStrategy() {}

SystemResult ModbusStrategy::SetConnectionParameters(const ModbusConnectionParameters &cConnectionParameters)
{
    _connectionParameters = cConnectionParameters;
    return SystemResult::SYSTEM_OK;
}
