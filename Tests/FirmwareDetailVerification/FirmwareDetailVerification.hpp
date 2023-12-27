#pragma once
#include <TestInterface.hpp>
#include <ModbusController.hpp>
#include <FirmwareDetailGUI.hpp>
#include <QPair>

class FirmwareDetailVerification : public TestInterface
{
public:
    FirmwareDetailVerification();
    ~FirmwareDetailVerification();

    void RunTest() override;

private:
    static constexpr const char TAG[] = "[FirmwareDetailVerification]";
    static constexpr const char _deviceName[] = "Auma";
    inline static const QString _cJsonFilePath = "/JSON/TestData.json";
    ModbusController *_mbController;
    ModbusStrategy *_mbStrategy;
    FirmwareDetailGUI _gui;

    void executeTest();
    void handleGUI();

    void testCompletedSuccessfully();
    void testFailed();

    QString getFirmwareVersion();
    QString extractFirwareVersion(QModbusReply *firmwareReply);

    QString getLanguage();
    QString extractLanguage(QModbusReply *firmwareReply);

    QPair<QString, int>getServiceInterface();
    QPair<QString, int>parseServiceInterface(QModbusReply *firmwareReply);
};
