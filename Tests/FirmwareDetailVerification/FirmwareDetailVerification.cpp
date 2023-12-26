#include <FirmwareDetailVerification.hpp>
#include <Logger.hpp>
#include <SystemResult.hpp>

#include <QCoreApplication>
#include <QTimer>
#include <QMessageBox>
#include <QThread>

static auto* logger = &Logger::GetInstance();

FirmwareDetailVerification::FirmwareDetailVerification()
{
    _mbController =  &ModbusController::GetInstance();
    if(!_mbController)
    {
        logger->LogCritical(TAG, "Modbus controller nullptr returned!");
        emit testFailed();
    }

    _mbStrategy = _mbController->GetInterfaceByName(_deviceName);
    if (!_mbStrategy)
    {
        logger->LogCritical(TAG, "Strategy nullptr returned!");
        emit testFailed();
    }
    else
    {
        QString appDirPath = QCoreApplication::applicationDirPath() + _cJsonFilePath;

        logger->LogInfo(TAG, appDirPath);
        _mbStrategy->LoadRegistersFromJSON(appDirPath);
    }


}
FirmwareDetailVerification::~FirmwareDetailVerification() {}

void FirmwareDetailVerification::RunTest()
{
    logger->LogCritical(TAG, "TEST NOT IMPLEMENTED");

    // Create a QThread object
    QThread* testThread = new QThread();

    // Move this object to the new thread
    this->moveToThread(testThread);

    // Connect the thread's started signal to the slot where you'll run your test
    connect(testThread, &QThread::started, this, &FirmwareDetailVerification::executeTest);

    // Connect any necessary cleanup after the test is done
    connect(this, &FirmwareDetailVerification::testCompletedSuccessfully, testThread, &QThread::quit);
    connect(this, &FirmwareDetailVerification::testFailed, testThread, &QThread::quit);
    connect(testThread, &QThread::finished, testThread, &QThread::deleteLater);

    // Start the thread
    testThread->start();

    (void)handleGUI();
}

void testCompletedSuccessfully()
{
    logger->LogCritical("[FirmwareDetailVerification]", "Test successful!");

    QMessageBox::critical(nullptr, "Test Success", "The firmware test was successful.");
}

void testFailed()
{
    logger->LogCritical("[FirmwareDetailVerification]", "Test failed!");

    QMessageBox::critical(nullptr, "Test Failure", "The firmware test failed. Please check the logs for more information.");
}

void FirmwareDetailVerification::executeTest()
{


    SystemResult status = SystemResult::SYSTEM_OK;

    logger->LogInfo(TAG, "Starting test for: " + _mbStrategy->GetDeviceName());

    if (!_mbStrategy->IsConnected())
    {
        logger->LogCritical(TAG, "Can't run test for disconnected device!");
        emit testFailed();
        return;
    }

    // QString firmwareVersion = getFirmwareVersion();

    // Complete the test and notify
    emit testCompletedSuccessfully();
}

void FirmwareDetailVerification::handleGUI()
{
    _gui.exec();
}

QString FirmwareDetailVerification::getFirmwareVersion()
{
    QString retVal;

    QModbusDataUnit data = _mbStrategy->GetQModbusDataUnitByName("Version");

    /* We don't need to ask for 20 bytes, the device will reply with data */
    data.setValueCount(9); // For debug purposes

    QModbusReply *firmwareReply = _mbStrategy->ReadData(data);
    if (!firmwareReply)
    {
        logger->LogCritical(TAG, "Failed to send read request for firmware version!");
        emit testFailed();
    }

    QEventLoop loop;
    connect(firmwareReply, &QModbusReply::finished, &loop, &QEventLoop::quit);
    QTimer::singleShot(5000, &loop, &QEventLoop::quit); // Timeout after 5000 ms
    loop.exec();

    if (firmwareReply->isFinished())
    {
        if (firmwareReply->error() == QModbusDevice::NoError)
        {
            QString firmwareVersion = extractFirwareVersion(firmwareReply);

            logger->LogDebug(TAG, "Firmware Version: " + firmwareVersion);
        }
        else
        {
            logger->LogCritical(TAG, "Failed to read firmware version from device: " + firmwareReply->errorString());
            emit testFailed();
        }
    }
    else
    {
        logger->LogCritical(TAG, "Timeout waiting for firmware version reply!");
        emit testFailed();
    }

    firmwareReply->deleteLater();
    return retVal;



    return QString();
}

QString FirmwareDetailVerification::extractFirwareVersion(QModbusReply *firmwareReply)
{
    constexpr uint8_t MAX_VERSION_LEN = 9;
    const QModbusDataUnit unit = firmwareReply->result();
    QVector<quint16> payload = unit.values();

    if (payload.size() < MAX_VERSION_LEN) // Since each register is 2 bytes, 20 bytes are 10 registers
    {
        logger->LogCritical(TAG, "Incomplete data received for firmware version!");
        emit testFailed();
        return QString();
    }

    // Assuming firmware version is stored in ASCII format
    QString firmwareVersion;
    for (int i = 0; i < MAX_VERSION_LEN; ++i)
    {
        firmwareVersion.append(QChar(payload[i]));
    }

    return firmwareVersion.trimmed(); // Trim any whitespace
}
