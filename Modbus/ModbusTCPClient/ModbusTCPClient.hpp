#pragma once
#include <ModbusClientStrategy.hpp>

class ModbusTCPClient : public ModbusClientStrategy
{
    Q_OBJECT
public:
    ModbusTCPClient();
    ~ModbusTCPClient();

    SystemResult Connect() override;
    SystemResult Disconnect() override;

    QModbusReply *ReadData(const QModbusDataUnit &cData) override;
    QModbusReply *WriteData(const QModbusDataUnit &cData) override;

private slots:
    void onModbusConnectionStateChanged(QModbusDevice::State state);

private:
    static constexpr const char* const MODBUS_TCP_DEVICE_NAME {"ModbusTCPDevice"};
    static constexpr const char* const CLASS_TAG {"[ModbusTCPClient]"};

    static constexpr const quint8 AUMA_UNIT_ID = 11;

    void initializeModbusClient();
    void connectSignalsAndSlots() const;
    void reconnect();

};



