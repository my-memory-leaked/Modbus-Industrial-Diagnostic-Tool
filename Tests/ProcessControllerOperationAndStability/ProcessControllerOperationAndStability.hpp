#pragma once
#include <TestInterface.hpp>

class ProcessControllerOperationAndStability : public TestInterface
{
public:
    ProcessControllerOperationAndStability();
    ~ProcessControllerOperationAndStability();

    void RunTest() override;

private:
    static constexpr const char TAG[] = "[ProcessControllerOperationAndStability]";
};

