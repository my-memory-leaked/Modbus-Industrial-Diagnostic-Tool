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

SystemResult LocalHostTest::_FieldbusOpen(ModbusStrategy *mbStrategy, bool state)
{
    SystemResult retVal = SystemResult::SYSTEM_OK;
    QModbusDataUnit writeUnit(QModbusDataUnit::Coils, 0, 1);
    writeUnit.setValue(0, state ? 0xFF00 : 0x0000);

    if (auto *reply = mbStrategy->WriteData(writeUnit)) { // 1 is the server address
        if (!reply->isFinished()) {
            // Connect signals to handle the reply
        }
    }
    ;

    return retVal;
}

SystemResult _FieldbusClose(ModbusStrategy *mbStrategy, bool state)
{
    SystemResult retVal = SystemResult::SYSTEM_OK;
    QModbusDataUnit query(QModbusDataUnit::Coils, 1, state);
    mbStrategy->WriteData(query);

    return retVal;
}

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

    // QString appDirPath = QCoreApplication::applicationDirPath() + _cJsonFilePath;

    // logger->LogInfo(TAG, appDirPath);
    // mbStrategy->LoadRegistersFromJSON(appDirPath);

    _FieldbusOpen(mbStrategy, true);
//     status = getFirmwareVersion(mbStrategy);
//     if( status != SystemResult::SYSTEM_OK)
//     {
//         logger->LogCritical(TAG, "Getting firmware version failed!");
// // TODO REACT
//     }

//     status = testOpenTo80Percent(mbStrategy);
//     if( status != SystemResult::SYSTEM_OK)
//     {
//         logger->LogCritical(TAG, "Setting actuator to 80% failed!");
//         // TODO REACT
//     }

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
        logger->LogDebug(TAG, "Modbus reply error: " + reply->errorString());
    }
}

SystemResult LocalHostTest::getFirmwareVersion(ModbusStrategy *mbStrategy)
{
    SystemResult retVal = SystemResult::SYSTEM_OK;

    QModbusDataUnit data = mbStrategy->GetQModbusDataUnitByName("Version");

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

SystemResult LocalHostTest::testOpenTo80Percent(ModbusStrategy *mbStrategy)
{
    return testActuatorPositioning(mbStrategy, 800);
}


SystemResult LocalHostTest::testActuatorPositioning(ModbusStrategy *mbStrategy, int targetPosition)
{
    SystemResult retVal = SystemResult::SYSTEM_OK;

    QModbusDataUnit setPositionUnit = mbStrategy->GetQModbusDataUnitByName("Positioner");
    QModbusDataUnit actualPositionUnit(setPositionUnit);
    setPositionUnit.setValue(0, targetPosition); // Set target position
    mbStrategy->WriteData(setPositionUnit);

    int actualPosition;

    while (true)
    {
        QModbusReply *positionReply = mbStrategy->ReadData(actualPositionUnit);

        QEventLoop loop;
        connect(positionReply, &QModbusReply::finished, &loop, &QEventLoop::quit);
        QTimer::singleShot(5000, &loop, &QEventLoop::quit); // Timeout after 5000 ms
        loop.exec();

        if (positionReply->isFinished())
        {
            if (positionReply->error() == QModbusDevice::NoError)
            {
                actualPosition = extractPositionValue(positionReply);
                if (positionReached(actualPosition, targetPosition))
                    break;
            }
            else
            {
                logger->LogCritical(TAG, "Modbus Error: " + positionReply->errorString());
                retVal = SystemResult::SYSTEM_ERROR;
                break;
            }
        }
        else
        {
            logger->LogCritical(TAG, "Timeout waiting for actual position reply!");
            retVal = SystemResult::SYSTEM_ERROR;
            break;
        }

        positionReply->deleteLater();
    }

    return retVal;
}


int LocalHostTest::extractPositionValue(QModbusReply *reply)
{
    const QModbusDataUnit unit = reply->result();
    if (unit.valueCount() > 0)
    {
        quint16 value = unit.value(0);
        return static_cast<int>(value);
    }
    else
    {
        logger->LogWarning(TAG, "No data in Modbus reply for position value.");
        return -1; // Indicate an error or invalid value
    }
}

bool LocalHostTest::positionReached(int actualPosition, int targetPosition, int tolerance)
{
    return std::abs(actualPosition - targetPosition) <= tolerance;
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
