#pragma once
#include <TestInterface.hpp>
#include <ProcessControlerGUI.hpp>
#include <ModbusController.hpp>

class ProcessControllerOperationAndStability : public TestInterface
{
public:
    ProcessControllerOperationAndStability();
    ~ProcessControllerOperationAndStability();

    void RunTest() override;

private:
    static constexpr const char TAG[] = "[ProcessControllerOperationAndStability]";
    static constexpr const char _deviceName[] = "Auma";
    inline static const QString _cJsonFilePath = "/JSON/TestData.json";

    ModbusController *_mbController;
    ModbusStrategy *_mbStrategy;
    ProcessControlerGUI _gui;

    int _warningCounter = 0;
    int _errorCounter = 0;

    QModbusReply *getMBReply(const QModbusDataUnit query);

    void executeTest();
    void handleGUI();

    void testCompletedSuccessfully();
    void testFailed();

    SystemResult readWarnings();
    SystemResult parseWarnings(QModbusReply *reply);
    SystemResult readErrors();
    SystemResult parseErrors(QModbusReply *reply);

    void positionerTest();
    SystemResult getActualPositionAndTorque();

    SystemResult fullyOpenAndCheckPosition();
    SystemResult fullyCloseAndCheckPosition();
    bool checkIfPositionReached(int position);

    SystemResult fieldbusOpen(bool state);
    SystemResult fieldbusClose(bool state);

    SystemResult testActuatorPositioning(int targetPosition);
    SystemResult writePosition(int position);

    int extractPositionValue(QModbusReply *reply);

    QPair<QString, QString> parsePositionAndTorque(const QModbusDataUnit &unit);
    QPair<int, int> parsePositionAndTorqueInt(const QModbusDataUnit &unit);

    QString modbusDataUnitToString(const QModbusDataUnit &unit);
};

