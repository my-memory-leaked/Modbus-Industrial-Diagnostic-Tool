#include <ModbusStrategy.hpp>

ModbusStrategy::ModbusStrategy(QObject *parent)
    : QObject(parent)
{
    static quint32 deviceID = -1;
    _deviceID = ++deviceID;
    _deviceName = "Default";
}


ModbusStrategy::~ModbusStrategy() {}
