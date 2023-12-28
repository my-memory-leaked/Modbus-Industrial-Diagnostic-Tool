#pragma once
#include <TestInterface.hpp>
#include <QString>
#include <QModbusReply>
#include <SystemResult.hpp>
#include <ModbusController.hpp>
#include <ModbusStrategy.hpp>
#include <ActuatorResponseGUI.hpp>
#include <QPair>

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

    ModbusController *_mbController;
    ModbusStrategy *_mbStrategy;
    ActuatorResponseGUI _gui;

    void executeTest();
    void handleGUI();

    void testCompletedSuccessfully();
    void testFailed();

    void toggleWarningDiode();
    void toggleErrorDiode();

    SystemResult readWarnings();
    SystemResult readErrors();

    SystemResult positionerTest();
    SystemResult checkPositionerRunning();

    SystemResult setPositioner(quint32 position);
    SystemResult resetPositioner(quint32 position);

    SystemResult getActualPositionAndTorque();

    SystemResult _FieldbusOpen(ModbusStrategy *mbStrategy, bool state);
    SystemResult _FieldbusClose(ModbusStrategy *mbStrategy, bool state);

    SystemResult testOpenTo80Percent(ModbusStrategy *mbStrategy);
    SystemResult testActuatorPositioning(ModbusStrategy *mbStrategy, int targetPosition);

    int extractPositionValue(QModbusReply *reply);
    bool positionReached(int actualPosition, int targetPosition, int tolerance = 10); // Tolerance can be adjusted



    QPair<QString, QString> parsePositionAndTorque(const QModbusDataUnit &unit);

    QString modbusDataUnitToString(const QModbusDataUnit &unit);
};

