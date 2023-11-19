#pragma once
#include <TestInterface.hpp>
#include <QString>

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
};
