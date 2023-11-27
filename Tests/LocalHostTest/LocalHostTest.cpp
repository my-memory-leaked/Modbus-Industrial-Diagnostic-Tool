#include <LocalHostTest.hpp>
#include <Logger.hpp>
#include <ModbusController.hpp>
#include <QCoreApplication>
#include <ModbusRegisterTypeMapper.hpp>

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

    QModbusDataUnit powerQuery = mbStrategy->GetQModbusDataUnitByName("Moc");
    QModbusReply* powerReply = mbStrategy->ReadData(powerQuery);

    if (powerReply)
    {
        if(powerReply->isFinished())
        {
            onPowerRegisterReceived(powerReply);
        } else
        {
            connect(powerReply, &QModbusReply::finished, this, [this, powerReply]() {
                onPowerRegisterReceived(powerReply);
            });        }
    }

}


QString LocalHostTest::GetDeviceName() const
{
    return QString(_deviceName);
}

void LocalHostTest::onPowerRegisterReceived(QModbusReply* reply)
{
    if (reply->error() == QModbusDevice::NoError)
    {
        const QModbusDataUnit unit = reply->result();

        logger->LogDebug(TAG, "Received data: Register Type: " + ModbusRegisterTypeMapper::RegisterTypeToString(unit.registerType()) +
                                  " Address: " + QString::number(unit.startAddress()) +
                                  " Value: " + QString::number(unit.value(0))); // Convert the value to a string
    }
    else
    {
        // Handle errors if necessary
        logger->LogDebug(TAG, "Modbus reply error: " + reply->errorString());
    }
}

