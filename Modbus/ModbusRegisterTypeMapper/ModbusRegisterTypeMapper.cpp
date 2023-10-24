#include <ModbusRegisterTypeMapper.hpp>

/* Initialize the mappings */
std::unordered_map<QModbusDataUnit::RegisterType, QString> ModbusRegisterTypeMapper::_registerTypeToStringMap = {
    {QModbusDataUnit::Invalid, "Invalid"},
    {QModbusDataUnit::DiscreteInputs, "DiscreteInputs"},
    {QModbusDataUnit::Coils, "Coils"},
    {QModbusDataUnit::InputRegisters, "InputRegisters"},
    {QModbusDataUnit::HoldingRegisters, "HoldingRegisters"}
};

std::unordered_map<QString, QModbusDataUnit::RegisterType> ModbusRegisterTypeMapper::_stringToRegisterTypeMap = {
    {"Invalid", QModbusDataUnit::Invalid},
    {"DiscreteInputs", QModbusDataUnit::DiscreteInputs},
    {"Coils", QModbusDataUnit::Coils},
    {"InputRegisters", QModbusDataUnit::InputRegisters},
    {"HoldingRegisters", QModbusDataUnit::HoldingRegisters}
};

ModbusRegisterTypeMapper::ModbusRegisterTypeMapper() {}

ModbusRegisterTypeMapper::~ModbusRegisterTypeMapper() {}


QString ModbusRegisterTypeMapper::RegisterTypeToString(const QModbusDataUnit::RegisterType& cType)
{
    auto it = _registerTypeToStringMap.find(cType);
    return (it != _registerTypeToStringMap.end()) ? it->second : "Unknown";
}

QModbusDataUnit::RegisterType ModbusRegisterTypeMapper::StringToRegisterType(const QString &cStr)
{
    auto it = _stringToRegisterTypeMap.find(cStr);
    return (it != _stringToRegisterTypeMap.end()) ? it->second : QModbusDataUnit::RegisterType::Invalid;
}
