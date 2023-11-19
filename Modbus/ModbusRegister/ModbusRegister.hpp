#pragma once
#include <QString>
#include <QModbusDataUnit>
#include <SystemResult.hpp>

class ModbusRegister
{
public:
    ModbusRegister();
    ModbusRegister(const QString& cRegisterName, const QModbusDataUnit::RegisterType& cQModbusRegisterType, const int& cAddress);
    ModbusRegister(const QString& cRegisterName, const QModbusDataUnit& cQModbusDataUnit);
    ModbusRegister(const ModbusRegister& cModbusRegister);
    bool operator==(const ModbusRegister& other) const;

    void SetName(const QString& cName);
    void SetModbusDataUnit(const QModbusDataUnit& cQModbusDataUnit);

    QString GetName() const;
    QModbusDataUnit GetQModbusdataUnit() const;

private:
    QString _registerName;
    QModbusDataUnit _qModbusDataUnit;
};
