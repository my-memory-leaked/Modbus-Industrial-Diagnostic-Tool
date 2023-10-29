#pragma once

#include <QListWidget>

#include <Singleton.hpp>
#include <SystemResult.hpp>
#include <ModbusStrategy.hpp>

#include <memory>
#include <map>

class ModbusController : public Singleton<ModbusController>
{
    friend class Singleton<ModbusController>;

public:

    SystemResult AddInterface(std::unique_ptr<ModbusStrategy> modbusStrategyPtr);
    SystemResult RemoveInterface(const QString& cDeviceName);
    ModbusStrategy* GetInterfaceByName(const QString& cDeviceName);

    void ConnectAllInterfaces();
    void DisconnectAllInterfaces();
    void ConnectInterface(const QString& cDeviceName);
    void DisconnectInterface(const QString& cDeviceName);

    void UpdateDeviceList(QListWidget* listWidget);

private:
    ModbusController();
    ~ModbusController();
    QModbusReply* readRegister(ModbusStrategy* modbusStrategyPtr, QModbusDataUnit::RegisterType cDataUnit, int startingAddress, quint16 numberOfRegisters);
    QModbusReply* writeRegister(ModbusStrategy* modbusStrategyPtr, QModbusDataUnit::RegisterType cDataUnit, int startingAddress, quint16 numberOfRegisters);

    std::map<QString, std::unique_ptr<ModbusStrategy>> _modbusInterfacesMap;

    static constexpr const char* TAG {"[ModbusController]"};
    static constexpr const char* NULL_PTR_MESSAGE {"Nullptr occurred!"};
};


