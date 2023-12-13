#include <LocalHostTest.hpp>
#include <Logger.hpp>
#include <ModbusController.hpp>
#include <QCoreApplication>
#include <ModbusRegisterTypeMapper.hpp>

#include <QDir>
#include <QFile>
#include <QTimer>

static auto *logger = &Logger::GetInstance();

LocalHostTest::LocalHostTest() {}
LocalHostTest::~LocalHostTest() {}

void LocalHostTest::RunTest()
{
    SystemResult status = SystemResult::SYSTEM_OK;

    auto *mbController = &ModbusController::GetInstance();
    ModbusStrategy *mbStrategy = nullptr;

    mbStrategy = mbController->GetInterfaceByName("localhost");
    if (!mbStrategy)
    {
        status = SystemResult::SYSTEM_ERROR;
        logger->LogCritical(TAG, "Strategy nullptr returned!");
        return;
    }

    logger->LogInfo(TAG, "Startring test for: " + mbStrategy->GetDeviceName());

    if (!mbStrategy->IsConnected())
    {
        status = SystemResult::SYSTEM_ERROR;
        logger->LogCritical(TAG, "Can't run test for disconnected device!");
        return;
    }

    QString appDirPath = QCoreApplication::applicationDirPath() + _cJsonFilePath;

    logger->LogInfo(TAG, appDirPath);
    mbStrategy->LoadRegistersFromJSON(appDirPath);



    getFirmwareVersion(mbStrategy);

    // status = getLanguageFromDevice(mbStrategy);
    // if (SystemResult::SYSTEM_OK != status)
    // {
    // }

    // QModbusDataUnit powerQuery = mbStrategy->GetQModbusDataUnitByName("FirmwareVersion");
    // QModbusReply *powerReply = mbStrategy->ReadData(powerQuery);

    // if (powerReply)
    // {
    //     if (powerReply->isFinished())
    //     {
    //         onPowerRegisterReceived(powerReply);
    //     }
    //     else
    //     {
    //         connect(powerReply, &QModbusReply::finished, this, [this, powerReply]()
    //                 { onPowerRegisterReceived(powerReply); });
    //     }
    // }
}

QString LocalHostTest::GetDeviceName() const
{
    return QString(_deviceName);
}

void LocalHostTest::onPowerRegisterReceived(QModbusReply *reply)
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

SystemResult LocalHostTest::getLanguageFromDevice(ModbusStrategy *mbStrategy)
{
    SystemResult retVal = SystemResult::SYSTEM_OK;
    QModbusDataUnit data = mbStrategy->GetQModbusDataUnitByName("Language");
    QModbusReply *languageReply = mbStrategy->ReadData(data);

    if (languageReply)
    {
        if (languageReply->isFinished())
        {
        }
        else
        {
            // QTimer::singleShot(timeoutDuration, this, [this, languageReply]() {
            //     if (!languageReply->isFinished())
            //     {
            //         // Handle timeout scenario
            //         retVal = SystemResult::SYSTEM_ERROR;
            //         logger->LogCritical(TAG, "Timeout occurred while reading language from device!");
            //     }
            // });
        }
    }
    else
    {
        retVal = SystemResult::SYSTEM_ERROR;
    }

    if (SystemResult::SYSTEM_OK != retVal)
        logger->LogCritical(TAG, "Filed to read language from device!");

    return retVal;
}

SystemResult LocalHostTest::getFirmwareVersion(ModbusStrategy *mbStrategy)
{
    SystemResult retVal = SystemResult::SYSTEM_OK;

    // Assuming GetQModbusDataUnitByName is a method that creates a QModbusDataUnit based on a register name
    QModbusDataUnit data = mbStrategy->GetQModbusDataUnitByName("FirmwareVersion");

    /* We need to ask for 20 bytes */
    data.setValueCount(20);

    QModbusReply *firmwareReply = mbStrategy->ReadData(data);
    if (!firmwareReply)
    {
        logger->LogCritical(TAG, "Failed to send read request for firmware version!");
        return SystemResult::SYSTEM_ERROR;
    }

    QEventLoop loop;
    connect(firmwareReply, &QModbusReply::finished, &loop, &QEventLoop::quit);
    QTimer::singleShot(5000, &loop, &QEventLoop::quit); // Timeout after 5000 ms
    loop.exec();

    if (firmwareReply->isFinished())
    {
        if (firmwareReply->error() == QModbusDevice::NoError)
        {
            const QModbusDataUnit unit = firmwareReply->result();
            QString firmwareVersion = modbusDataUnitToString(unit);
            logger->LogDebug(TAG, "Firmware Version: " + firmwareVersion);
        }
        else
        {
            logger->LogCritical(TAG, "Failed to read firmware version from device: " + firmwareReply->errorString());
            retVal = SystemResult::SYSTEM_ERROR;
        }
    }
    else
    {
        logger->LogCritical(TAG, "Timeout waiting for firmware version reply!");
        retVal = SystemResult::SYSTEM_ERROR;
    }

    firmwareReply->deleteLater();
    return retVal;
}

QString LocalHostTest::modbusDataUnitToString(const QModbusDataUnit &unit)
{
    QString result;
    for (uint i = 0; i < unit.valueCount(); ++i) {
        quint16 value = unit.value(i);
        result.append(QChar(value));
    }
    return result;
}
