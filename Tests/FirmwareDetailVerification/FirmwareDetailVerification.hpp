#pragma once
#include <TestInterface.hpp>
#include <ModbusController.hpp>

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

    void handleTestFailure();
    QString getFirmwareVersion();
    QString extractFirwareVersion(QModbusReply *firmwareReply);

};
