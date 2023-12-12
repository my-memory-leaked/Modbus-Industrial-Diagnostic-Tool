#pragma once
#include <TestInterface.hpp>
#include <QString>
#include <QModbusReply>

class LocalHostTest : public TestInterface
{
public:
    LocalHostTest();
    ~LocalHostTest();

    void RunTest() override;

    QString GetDeviceName() const;
private:
    static constexpr const char TAG[] = "[LocalHostTest]";
    static constexpr const char _deviceName[] = "Termometr Przemysłowy";
    inline static const QString _cJsonFilePath = "/JSON/TestData.json";

private slots:
    void onPowerRegisterReceived(QModbusReply* reply);
};
