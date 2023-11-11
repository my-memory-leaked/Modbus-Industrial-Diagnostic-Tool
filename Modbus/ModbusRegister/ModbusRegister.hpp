#pragma once
#include <QString>
#include <QModbusDataUnit>

class ModbusRegister
{
public:
    ModbusRegister(const QString& cRegisterName, const QModbusDataUnit::RegisterType& cQModbusRegisterType, const int& cAddress);
    ModbusRegister(const QString& cRegisterName, const QModbusDataUnit& cQModbusDataUnit);

    QString GetName() const;

    bool operator==(const ModbusRegister& other) const;

private:
    QString _registerName;
    QModbusDataUnit _qModbusDataUnit;
};
