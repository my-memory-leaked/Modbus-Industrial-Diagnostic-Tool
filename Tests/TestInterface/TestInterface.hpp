#pragma once

class TestInterface
{
public:
    TestInterface();
    virtual ~TestInterface();

    virtual void RunTest() = 0;
};
