#include <ModbusStateMapper.hpp>

/* Initialize the mappings */
std::unordered_map<QModbusDevice::State, QString> ModbusStateMapper::_stateToStringMap = {
    {QModbusDevice::UnconnectedState, "Disconnected"},
    {QModbusDevice::ConnectingState, "Connecting"},
    {QModbusDevice::ConnectedState, "Connected"},
    {QModbusDevice::ClosingState, "Closing"}
};

std::unordered_map<QString, QModbusDevice::State> ModbusStateMapper::_stringToStateMap = {
    {"Disconnected", QModbusDevice::UnconnectedState},
    {"Connecting", QModbusDevice::ConnectingState},
    {"Connected", QModbusDevice::ConnectedState},
    {"Closing", QModbusDevice::ClosingState}
};

ModbusStateMapper::ModbusStateMapper() {}

ModbusStateMapper::~ModbusStateMapper() {}

QString ModbusStateMapper::StateToString(const QModbusDevice::State& cState)
{
    auto it = _stateToStringMap.find(cState);
    return (it != _stateToStringMap.end()) ? it->second : "Unknown";
}

QModbusDevice::State ModbusStateMapper::StringToState(const QString &cStr)
{
    auto it = _stringToStateMap.find(cStr);
    return (it != _stringToStateMap.end()) ? it->second : QModbusDevice::UnconnectedState;
}
