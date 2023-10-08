#include <ModbusStrategy.hpp>

ModbusStrategy::ModbusStrategy()
{
    static quint32 deviceID = -1;
    _deviceID = ++deviceID;
    _deviceName = "Default";
}


ModbusStrategy::~ModbusStrategy() {}



SystemResult ModbusStrategy::SetConnectionParameters(const ModbusConnectionParameters &cConnectionParameters)
{
    _connectionParameters = cConnectionParameters;
    return SystemResult::SYSTEM_OK;
}
