#include <FirmwareDetailVerification.hpp>
#include <Logger.hpp>

static auto* logger = &Logger::GetInstance();

FirmwareDetailVerification::FirmwareDetailVerification() {}
FirmwareDetailVerification::~FirmwareDetailVerification() {}

void FirmwareDetailVerification::RunTest()
{
    logger->LogCritical(TAG, "TEST NOT IMPLEMENTED");
}
