#pragma once
#include <DeviceInterface.hpp>
#include <ModbusRegister.hpp>

class ModbusDeviceInterface: public DeviceInterface
{
public:
    SystemResult AddRegister(const ModbusRegister& cModbusRegister);
    ModbusRegister GetRegisterByName(const QString& cRegisterName);

    SystemResult LoadRegistersFromJSON(const QString& cFilePath);

protected:
    QVector<ModbusRegister> _deviceRegisters;

    static constexpr const char TAG[] {"[ModbusDeviceInterface]"};
};
