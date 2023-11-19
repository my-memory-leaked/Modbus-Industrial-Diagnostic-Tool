#include <ModbusDeviceInterface.hpp>
#include <Logger.hpp>
#include <JSONToModbusRegisterConventer.hpp>

auto* logger = &Logger::GetInstance();


SystemResult ModbusDeviceInterface::AddRegister(const ModbusRegister& cModbusRegister)
{
    SystemResult result = SystemResult::SYSTEM_OK;

    for (const ModbusRegister& existingRegister : _deviceRegisters)
    {
        if (existingRegister == cModbusRegister)
        {
            logger->LogDebug(TAG, "Attempted to add a duplicate ModbusRegister " + cModbusRegister.GetName() + ".");
            result = SystemResult::SYSTEM_ERROR;
        }
    }

    if (SystemResult::SYSTEM_OK == result)
    {
        _deviceRegisters.append(cModbusRegister);
        logger->LogInfo(TAG, "ModbusRegister " + cModbusRegister.GetName() + "added successfully.");
    }
    else
    {
        logger->LogWarning(TAG, "Failed to add ModbusRegister " + cModbusRegister.GetName() + "due to duplication.");
    }

    return result;
}

ModbusRegister ModbusDeviceInterface::GetRegisterByName(const QString& cRegisterName)
{
    ModbusRegister retVal;
    logger->LogInfo(TAG, "Requested to get register " + cRegisterName);

    for (ModbusRegister& existingRegister : _deviceRegisters)
    {
        if (existingRegister.GetName() == cRegisterName)
        {
            retVal = existingRegister;
            logger->LogInfo(TAG, "Found register " + cRegisterName);
        }
    }

    return retVal;
}

SystemResult ModbusDeviceInterface::LoadRegistersFromJSON(const QString& cFilePath)
{
    SystemResult retVal = SystemResult::SYSTEM_OK;

    auto* jsonConventer = &JSONToModbusRegisterConventer::GetInstance();
    QVector<ModbusRegister> registers = jsonConventer->FromJsonFile(cFilePath);

    if (registers.isEmpty())
    {
        retVal = SystemResult::SYSTEM_ERROR;
        logger->LogCritical(TAG, "Could not load modbus register data from JSON!");
    }

    if ( SystemResult::SYSTEM_OK == retVal )
         _deviceRegisters = registers;

    return retVal;
}
