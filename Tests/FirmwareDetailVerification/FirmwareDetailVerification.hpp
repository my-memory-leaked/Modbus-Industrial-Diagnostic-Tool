#pragma once
#include <TestInterface.hpp>
#include <ModbusController.hpp>
#include <FirmwareDetailGUI.hpp>

class FirmwareDetailVerification : public TestInterface
{
    Q_OBJECT
public:
    FirmwareDetailVerification();
    ~FirmwareDetailVerification();

    void RunTest() override;

signals:
    void testCompletedSuccessfully();
    void testFailed();

private:
    static constexpr const char TAG[] = "[FirmwareDetailVerification]";
    static constexpr const char _deviceName[] = "Auma";
    inline static const QString _cJsonFilePath = "/JSON/TestData.json";
    ModbusController *_mbController;
    ModbusStrategy *_mbStrategy;
    FirmwareDetailGUI _gui;

    void executeTest();
    void handleGUI();

    void handleTestFailure();
    QString getFirmwareVersion();
    QString extractFirwareVersion(QModbusReply *firmwareReply);

};
