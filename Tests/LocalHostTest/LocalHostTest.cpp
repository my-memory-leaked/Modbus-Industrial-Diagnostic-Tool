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

    QString appDirPath = QCoreApplication::applicationDirPath();
    QString jsonFilePath = appDirPath + "/JSON/TestData.json"; // Adjust the relative path as needed
    logger->LogInfo(TAG, jsonFilePath);

    // QString jsonFilePath = "D:\\Repositories\\Modbus-Industrial-Diagnostic-Tool\\Modbus-Industrial-Diagnostic-Tool\\JSON\\TestData.json";
    // logger->LogInfo(TAG, "Test json path: " + jsonFilePath);

    mbStrategy->LoadRegistersFromJSON(jsonFilePath);


    QModbusDataUnit data = mbStrategy->GetQModbusDataUnitByName("Siema");

    mbStrategy->ReadData(data);

}


QString LocalHostTest::GetDeviceName() const
{
    return QString(_deviceName);
}
