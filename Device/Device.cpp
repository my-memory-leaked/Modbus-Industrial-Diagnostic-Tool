#include <Device.hpp>
#include <Logger.hpp>

auto* logger = &Logger::GetInstance();

Device::Device(const QString& cDeviceName) : _deviceName(cDeviceName)
{
}

SystemResult Device::AddRegister(const ModbusRegister& cModbusRegister)
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

ModbusRegister Device::GetRegisterByName(const QString& cRegisterName)
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
