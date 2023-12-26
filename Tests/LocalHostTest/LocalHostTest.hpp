#pragma once
#include <TestInterface.hpp>
#include <QString>
#include <QModbusReply>
#include <SystemResult.hpp>
#include <ModbusStrategy.hpp>

class LocalHostTest : public TestInterface
{
public:
    LocalHostTest();
    ~LocalHostTest();

    void RunTest() override;

    QString GetDeviceName() const;
private slots:
    void onPowerRegisterReceived(QModbusReply *reply);

private:
    static constexpr const char TAG[] = "[LocalHostTest]";
    static constexpr const char _deviceName[] = "Auma";
    inline static const QString _cJsonFilePath = "/JSON/TestData.json";

    SystemResult getFirmwareVersion(ModbusStrategy *mbStrategySystemResult);
    SystemResult testOpenTo80Percent(ModbusStrategy *mbStrategy);
    SystemResult testActuatorPositioning(ModbusStrategy *mbStrategy, int targetPosition);

    int extractPositionValue(QModbusReply *reply);
    bool positionReached(int actualPosition, int targetPosition, int tolerance = 10);

    QString modbusDataUnitToString(const QModbusDataUnit &unit);
    SystemResult _FieldbusOpen(ModbusStrategy *mbStrategy, bool state);
};
