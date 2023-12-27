#include <ProcessControllerOperationAndStability.hpp>
#include <Logger.hpp>

static auto* logger = &Logger::GetInstance();

ProcessControllerOperationAndStability::ProcessControllerOperationAndStability() {}

ProcessControllerOperationAndStability::~ProcessControllerOperationAndStability() {}


void ProcessControllerOperationAndStability::RunTest()
{
    logger->LogCritical(TAG, "TEST NOT IMPLEMENTED!!!");
}
