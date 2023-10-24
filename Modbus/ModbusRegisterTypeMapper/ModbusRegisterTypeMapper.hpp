#pragma once

#include <Singleton.hpp>
#include <QModbusDataUnit>
#include <unordered_map>
#include <QString>

class ModbusRegisterTypeMapper : public Singleton<ModbusRegisterTypeMapper>
{
    friend class Singleton<ModbusRegisterTypeMapper>;
public:

    static QString RegisterTypeToString(const QModbusDataUnit::RegisterType& cType);
    static QModbusDataUnit::RegisterType StringToRegisterType(const QString &cStr);


private:
    ModbusRegisterTypeMapper();
    ~ModbusRegisterTypeMapper();

    static std::unordered_map<QModbusDataUnit::RegisterType, QString> _registerTypeToStringMap;
    static std::unordered_map<QString, QModbusDataUnit::RegisterType> _stringToRegisterTypeMap;
};
