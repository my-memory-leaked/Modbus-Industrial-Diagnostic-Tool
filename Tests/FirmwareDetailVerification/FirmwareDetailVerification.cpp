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
        testFailed();
    }

    _mbStrategy = _mbController->GetInterfaceByName(_deviceName);
    if (!_mbStrategy)
    {
        logger->LogCritical(TAG, "Strategy nullptr returned!");
        testFailed();
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
    _gui.SetProgressBar(0);
    // Create a QThread object
    QThread* testThread = new QThread();

    // Connect the thread's started signal to the slot where you'll run your test
    connect(testThread, &QThread::started, this, &FirmwareDetailVerification::executeTest);
    connect(testThread, &QThread::finished, testThread, &QThread::deleteLater);

    // Start the thread
    testThread->start();

    (void)handleGUI();

    delete testThread;
}

void FirmwareDetailVerification::testCompletedSuccessfully()
{
    logger->LogCritical(TAG, "Test successful!");

    QMessageBox::information(nullptr, "Test Success", "The firmware test was successful.");
}

void FirmwareDetailVerification::testFailed()
{
    logger->LogCritical(TAG, "Test failed!");

    QMessageBox::critical(nullptr, "Test Failure", "The firmware test failed. Please check the logs for more information.");
}

void FirmwareDetailVerification::executeTest()
{
    SystemResult status = SystemResult::SYSTEM_OK;

    logger->LogInfo(TAG, "Starting test for: " + _mbStrategy->GetDeviceName());

    if (!_mbStrategy->IsConnected())
    {
        logger->LogCritical(TAG, "Can't run test for disconnected device!");
        testFailed();
        return;
    }
    _gui.SetProgressBar(10);

    QString firmwareVersion = getFirmwareVersion();

    if (firmwareVersion.isEmpty())
    {
        testFailed();
        return;
    }
    else
    {
        _gui.SetFirmwareVersionLabel(firmwareVersion);
    }
    _gui.SetProgressBar(35);



    _gui.SetProgressBar(100);
    // Complete the test and notify
    testCompletedSuccessfully();
}

void FirmwareDetailVerification::handleGUI()
{
    if(_gui.exec() == QDialog::Accepted)
    {

    }
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
            retVal = firmwareVersion;
        }
        else
        {
            logger->LogCritical(TAG, "Failed to read firmware version from device: " + firmwareReply->errorString());
            testFailed();
        }
    }
    else
    {
        logger->LogCritical(TAG, "Timeout waiting for firmware version reply!");
        testFailed();
    }

    firmwareReply->deleteLater();
    return retVal;
}

QString FirmwareDetailVerification::extractFirwareVersion(QModbusReply *firmwareReply)
{
    constexpr uint8_t MAX_VERSION_LEN = 9;
    const QModbusDataUnit unit = firmwareReply->result();
    QVector<quint16> payload = unit.values();

    if (payload.size() < MAX_VERSION_LEN) // Since each register is 2 bytes, 20 bytes are 10 registers
    {
        logger->LogCritical(TAG, "Incomplete data received for firmware version!");
        testFailed();
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
