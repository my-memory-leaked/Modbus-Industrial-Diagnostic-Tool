#pragma once
#include <QString>
#include <QVector>
#include <SystemResult.hpp>
#include <ModbusRegister.hpp>


class Device
{
public:

    Device(const QString& cDeviceName);

    SystemResult AddRegister(const ModbusRegister& cModbusRegister);
    ModbusRegister GetRegisterByName(const QString& cRegisterName);

private:
    QString _deviceName;
    QVector<ModbusRegister> _deviceRegisters;

    static constexpr const char TAG[] {"[Device]"};
};
