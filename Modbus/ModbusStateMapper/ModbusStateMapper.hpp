#pragma once

#include <QString>
#include <QModbusDevice>
#include <Singleton.hpp>
#include <unordered_map>

class ModbusStateMapper : public Singleton<ModbusStateMapper>
{
    friend class Singleton<ModbusStateMapper>;

public:
    static QString StateToString(const QModbusDevice::State& cState);
    static QModbusDevice::State StringToState(const QString &cStr);

private:
    ModbusStateMapper();
    ~ModbusStateMapper();

    static std::unordered_map<QModbusDevice::State, QString> _stateToStringMap;
    static std::unordered_map<QString, QModbusDevice::State> _stringToStateMap;
};
