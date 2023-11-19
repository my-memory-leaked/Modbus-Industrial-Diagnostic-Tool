#include <LocalHostTest.hpp>
#include <Logger.hpp>
#include <ModbusController.hpp>

static auto* logger = &Logger::GetInstance();

LocalHostTest::LocalHostTest() {}
LocalHostTest::~LocalHostTest() {}


void LocalHostTest::RunTest()
{
    auto* mbController = &ModbusController::GetInstance();
    ModbusStrategy* mbStrategy = nullptr;

    mbStrategy = mbController->GetInterfaceByName("localhost");
    if ( !mbStrategy )
    {
        logger->LogCritical(TAG, "Strategy nullptr returned!");
        return;
    }

    logger->LogInfo(TAG, "Startring test for: "+ mbStrategy->GetDeviceName());


    if ( !mbStrategy->IsConnected() )
    {
        logger->LogCritical(TAG, "Can't start test for disconnected device!");
        return;
    }




}


QString LocalHostTest::GetDeviceName() const
{
    return QString(_deviceName);
}
