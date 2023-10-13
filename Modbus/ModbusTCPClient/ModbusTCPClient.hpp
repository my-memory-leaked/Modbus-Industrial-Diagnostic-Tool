#pragma once
#include <ModbusClientStrategy.hpp>

class ModbusTCPClient : public ModbusClientStrategy
{
public:
    ModbusTCPClient();
    ~ModbusTCPClient();

    SystemResult Connect() override;
    SystemResult Disconnect() override;

    SystemResult ReadData(const QModbusDataUnit &cData) override;
    SystemResult WriteData(const QModbusDataUnit &cData) override;

private:
    static constexpr const char* const MODBUS_TCP_DEVICE_NAME {"ModbusTCPDevice"};
    static constexpr const char* const CLASS_TAG {"[ModbusTCPClient]"};
};



