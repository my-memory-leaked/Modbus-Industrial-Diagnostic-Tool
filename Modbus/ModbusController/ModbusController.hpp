#pragma once

#include <Singleton.hpp>
#include <SystemResult.hpp>
#include <ModbusStrategy/ModbusStrategy.hpp>
#include <QThread>

class ModbusController : public QThread, public Singleton<ModbusController>
{
    Q_OBJECT
    friend class Singleton<ModbusController>;
public:

    SystemResult AddInterface(ModbusStrategy* modbusStrategyPtr);
    SystemResult RemoveInterface(ModbusStrategy* modbusStrategyPtr);

    void InitializeInterfaces();

    SystemResult SetInterfaceParameters(ModbusStrategy* modbusStrategyPtr, const ModbusConnectionParameters &cConnectionParameters);

public slots:
    void start(Priority priority = InheritPriority);
    void terminate();


private:
    ModbusController();
    ~ModbusController();

    QList<ModbusStrategy*> _modbusInterfacesList;

    static constexpr const char* TAG {"[ModbusController]"};
    static constexpr const char* NULL_PTR_MESSAGE {"Nullptr occured!"};

    QModbusReply *readRegister(ModbusStrategy* modbusStrategyPtr, QModbusDataUnit::RegisterType cDataUnit, int startingAddress, quint16 numberOfRegisters);
    QModbusReply *writeRegister(ModbusStrategy* modbusStrategyPtr, QModbusDataUnit::RegisterType cDataUnit, int startingAddress, quint16 numberOfRegisters);

};

