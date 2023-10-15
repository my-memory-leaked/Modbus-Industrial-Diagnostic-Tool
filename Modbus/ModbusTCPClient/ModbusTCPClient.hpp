#pragma once
#include <QObject>
#include <ModbusClientStrategy.hpp>

class ModbusTCPClient : public ModbusClientStrategy
{
    Q_OBJECT
public:
    ModbusTCPClient();
    ~ModbusTCPClient();

    SystemResult Connect() override;
    SystemResult Disconnect() override;

    SystemResult ReadData(const QModbusDataUnit &cData) override;
    SystemResult WriteData(const QModbusDataUnit &cData) override;

    static constexpr const char* const MODBUS_TCP_DEVICE_NAME {"ModbusTCPDevice"};
private slots:
    void onModbusConnectionStateChanged(QModbusDevice::State state); // Slot to handle state changes

private:
    static constexpr const char* const CLASS_TAG {"[ModbusTCPClient]"};

    void initializeModbusClient();
    void connectSignalsAndSlots() const;

};



