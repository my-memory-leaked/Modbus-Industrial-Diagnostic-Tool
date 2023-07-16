#include "ModbusStrategy.hpp"

ModbusStrategy::ModbusStrategy(const ModbusConnectionParameters &cConnectionParams, QObject *parent)
    : QObject(parent), _connectionParams(cConnectionParams)
{
}
