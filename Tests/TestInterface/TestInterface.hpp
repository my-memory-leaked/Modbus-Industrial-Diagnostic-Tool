#pragma once
#include <QObject>

class TestInterface : public QObject
{
    Q_OBJECT;
public:
    TestInterface();
    virtual ~TestInterface();

    virtual void RunTest() = 0;
};
