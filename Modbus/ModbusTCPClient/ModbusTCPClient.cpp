#include <ModbusTCPClient.hpp>

ModbusTCPClient::ModbusTCPClient()
{

}
ModbusTCPClient::~ModbusTCPClient()
{

}

SystemResult SetConnectionParameters(const ModbusConnectionParameters &cConnectionParameters)
{
    return SystemResult::SYSTEM_ERROR;
}

SystemResult ReadData(const QModbusDataUnit &cData)
{
    return SystemResult::SYSTEM_ERROR;
}

SystemResult WriteData(const QModbusDataUnit &cData)
{
    return SystemResult::SYSTEM_ERROR;
}
