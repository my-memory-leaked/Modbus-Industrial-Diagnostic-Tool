#include <ModbusTCPClient.hpp>

ModbusTCPClient::ModbusTCPClient()
{

}
ModbusTCPClient::~ModbusTCPClient()
{

}

SystemResult ModbusTCPClient::SetConnectionParameters(const ModbusConnectionParameters &cConnectionParameters)
{
    return SystemResult::SYSTEM_ERROR;
}

SystemResult ModbusTCPClient::ReadData(const QModbusDataUnit &cData)
{
    return SystemResult::SYSTEM_ERROR;
}

SystemResult ModbusTCPClient::WriteData(const QModbusDataUnit &cData)
{
    return SystemResult::SYSTEM_ERROR;
}
