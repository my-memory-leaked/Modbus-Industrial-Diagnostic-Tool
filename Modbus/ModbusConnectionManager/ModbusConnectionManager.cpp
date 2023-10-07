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
    _modbusStrategy->Connect();
}
void ModbusConnectionManager::CloseConnection()
{
    _modbusStrategy->Disconnect();
}
