#include <LocalHostTest.hpp>
#include <Logger.hpp>
#include <ModbusController.hpp>
#include <QCoreApplication>

#include <QDir>
#include <QFile>

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

    QString appDirPath = QCoreApplication::applicationDirPath() + _cJsonFilePath;

    logger->LogInfo(TAG, appDirPath);
    mbStrategy->LoadRegistersFromJSON(appDirPath);

    QModbusDataUnit data = mbStrategy->GetQModbusDataUnitByName("Siema");
    mbStrategy->ReadData(data);

    QModbusDataUnit data2 = mbStrategy->GetQModbusDataUnitByName("Prędkość Obrotowa");
    mbStrategy->ReadData(data2);

}


QString LocalHostTest::GetDeviceName() const
{
    return QString(_deviceName);
}
