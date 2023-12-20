#pragma once
#include <TestInterface.hpp>

class FirmwareDetailVerification : public TestInterface
{
public:
    FirmwareDetailVerification();
    ~FirmwareDetailVerification();

    void RunTest() override;

private:
    static constexpr const char TAG[] = "[FirmwareDetailVerification]";

};
