#pragma once

#include <QListWidget>

#include <Singleton.hpp>
#include <SystemResult.hpp>
#include <ModbusStrategy.hpp>

#include <memory>
#include <map>

class ModbusController : public QObject, public Singleton<ModbusController>
{
    Q_OBJECT
    friend class Singleton<ModbusController>;

public:

    SystemResult AddInterface(std::shared_ptr<ModbusStrategy> modbusStrategyPtr);
    SystemResult RemoveInterface(const QString& cDeviceName);
    ModbusStrategy* GetInterfaceByName(const QString& cDeviceName);
    const std::map<QString, std::shared_ptr<ModbusStrategy>> &GetInterfacesMap() const;

    void ConnectAllInterfaces();
    void DisconnectAllInterfaces();
    void ConnectInterface(const QString& cDeviceName);
    void DisconnectInterface(const QString& cDeviceName);

private:
    ModbusController();
    ~ModbusController();
    std::map<QString, std::shared_ptr<ModbusStrategy>> _modbusInterfacesMap;
    static constexpr const char* TAG {"[ModbusController]"};
    static constexpr const char* NULL_PTR_MESSAGE {"Nullptr occurred!"};

};


