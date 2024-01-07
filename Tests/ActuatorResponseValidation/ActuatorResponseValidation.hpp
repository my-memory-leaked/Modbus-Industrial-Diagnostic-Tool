#pragma once
#include <TestInterface.hpp>
#include <QString>
#include <QModbusReply>
#include <SystemResult.hpp>
#include <ModbusController.hpp>
#include <ModbusStrategy.hpp>
#include <ActuatorResponseGUI.hpp>
#include <QPair>
#include <QModbusDataUnit>

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

    SystemResult testOpenTo80Percent(ModbusStrategy *mbStrategy);
    SystemResult testActuatorPositioning(ModbusStrategy *mbStrategy, int targetPosition);

    int extractPositionValue(QModbusReply *reply);
    bool positionReached(int actualPosition, int targetPosition, int tolerance = 10); // Tolerance can be adjusted



    QPair<QString, QString> parsePositionAndTorque(const QModbusDataUnit &unit);
    QPair<int, int> parsePositionAndTorqueInt(const QModbusDataUnit &unit);


    QString modbusDataUnitToString(const QModbusDataUnit &unit);
};

