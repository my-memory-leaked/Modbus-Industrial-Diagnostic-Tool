#pragma once
#include <TestInterface.hpp>
#include <QString>
#include <QModbusReply>
#include <SystemResult.hpp>
#include <ModbusStrategy.hpp>


class ActuatorResponseValidation : public TestInterface
{
public:
    ActuatorResponseValidation();
    ~ActuatorResponseValidation();

    void RunTest() override;


private:
    static constexpr const char TAG[] = "[LocalHostTest]";
    static constexpr const char _deviceName[] = "Auma";
    inline static const QString _cJsonFilePath = "/JSON/TestData.json";

    SystemResult testOpenTo80Percent(ModbusStrategy *mbStrategy);
    SystemResult testActuatorPositioning(ModbusStrategy *mbStrategy, int targetPosition);
    int extractPositionValue(QModbusReply *reply);
    bool positionReached(int actualPosition, int targetPosition, int tolerance = 10); // Tolerance can be adjusted
    QString modbusDataUnitToString(const QModbusDataUnit &unit);

};

