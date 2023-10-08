#pragma once

#include <Singleton.hpp>
#include <SystemResult.hpp>
#include <ModbusStrategy/ModbusStrategy.hpp>

class ModbusController : public Singleton<ModbusController>
{
    friend class Singleton<ModbusController>;
public:

    SystemResult AddInterface(ModbusStrategy* modbusStrategyPtr);
    SystemResult RemoveInterface(ModbusStrategy* modbusStrategyPtr);

    QModbusDataUnit ReadRegister(QModbusDataUnit::RegisterType cDataUnit, int startingAddress, quint16 numberOfRegisters);
    QModbusDataUnit WriteRegister(QModbusDataUnit::RegisterType cDataUnit, int startingAddress, quint16 numberOfRegisters);


private:
    ModbusController();
    ~ModbusController();

    QList<ModbusStrategy*> _modbusInterfacesList;

    static constexpr const char* TAG {"ModbusController"};
    static constexpr const char* NULL_PTR_MESSAGE {"Nullptr occured!"};

};

