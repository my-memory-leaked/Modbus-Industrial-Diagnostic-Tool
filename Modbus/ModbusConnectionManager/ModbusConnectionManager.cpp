#include "ModbusConnectionManager.hpp"

ModbusConnectionManager::ModbusConnectionManager(ModbusStrategy *modbusStrategy, QObject *parent)
    : QObject(parent), _modbusStrategy(modbusStrategy)
{
}

ModbusConnectionManager::~ModbusConnectionManager()
{
}

void ModbusConnectionManager::Start()
{
    _modbusStrategy->Start();
}

void ModbusConnectionManager::SendData(const QModbusDataUnit &data)
{
    _modbusStrategy->SendData(data);
}

void ModbusConnectionManager::Disconnect()
{
    _modbusStrategy->disconnect();
}
