#pragma once
#include <QModbusDataUnit>

class ModbusCommand
{
public:
    ModbusCommand();

    ModbusCommand(QModbusDataUnit::RegisterType type, int newStartAddress, quint16 newValueCount) :_mbDataUnit(){}

    ModbusCommand(QModbusDataUnit::RegisterType type, int newStartAddress, const QList<quint16> &newValues)
    {}

private:
    QModbusDataUnit _mbDataUnit;
};
