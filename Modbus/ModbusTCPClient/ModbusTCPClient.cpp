#include <ModbusTCPClient.hpp>

ModbusTCPClient::ModbusTCPClient()
{
    this->SetDeviceName(MODBUS_TCP_DEVICE_NAME);
}
ModbusTCPClient::~ModbusTCPClient()
{

}

SystemResult ModbusTCPClient::Connect()
{
    return SystemResult::SYSTEM_ERROR;
}

SystemResult ModbusTCPClient::Disconnect()
{
    return SystemResult::SYSTEM_ERROR;
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
