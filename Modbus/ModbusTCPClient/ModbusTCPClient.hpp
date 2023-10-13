#pragma once
#include <QObject>
#include <ModbusStrategy.hpp>


class ModbusTCPClient : public ModbusStrategy
{
public:
    ModbusTCPClient();
    ~ModbusTCPClient();

    SystemResult Connect() override;
    SystemResult Disconnect() override;

    SystemResult SetConnectionParameters(const ModbusConnectionParameters &cConnectionParameters) override;

    SystemResult ReadData(const QModbusDataUnit &cData) override;
    SystemResult WriteData(const QModbusDataUnit &cData) override;

private:

    static constexpr const char* const MODBUS_TCP_DEVICE_NAME {"ModbusTCPDevice"};
};



