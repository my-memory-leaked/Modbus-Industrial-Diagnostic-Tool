#include <ModbusController.hpp>
#include <Logger.hpp>

static auto* logger = &Singleton<Logger>::GetInstance();

ModbusController::ModbusController() {}
ModbusController::~ModbusController() {}

SystemResult ModbusController::AddInterface(ModbusStrategy* modbusStrategyPtr)
{
    SystemResult retVal = SystemResult::SYSTEM_OK;

    if(!modbusStrategyPtr)
        retVal = SystemResult::SYSTEM_ERROR;

    if(SystemResult::SYSTEM_OK != retVal)
        logger->LogCritical(TAG, NULL_PTR_MESSAGE);

    if(SystemResult::SYSTEM_OK == retVal)
        _modbusInterfacesList.append(modbusStrategyPtr);

    if(SystemResult::SYSTEM_OK != retVal && modbusStrategyPtr)
        logger->LogCritical(TAG, "Adding interface " + modbusStrategyPtr->GetDeviceName() + " failed!");

    return retVal;
}

SystemResult ModbusController::RemoveInterface(ModbusStrategy* modbusStrategyPtr)
{
    SystemResult retVal = SystemResult::SYSTEM_OK;

    if(!modbusStrategyPtr)
        retVal = SystemResult::SYSTEM_ERROR;

    if(SystemResult::SYSTEM_OK != retVal)
        logger->LogCritical(TAG, NULL_PTR_MESSAGE);

    if(SystemResult::SYSTEM_OK == retVal)
        _modbusInterfacesList.removeAt(modbusStrategyPtr->GetDeviceID());

    if(SystemResult::SYSTEM_OK != retVal && modbusStrategyPtr)
        logger->LogCritical(TAG, "Removing interface " + modbusStrategyPtr->GetDeviceName() + " failed!");

    return retVal;
}
