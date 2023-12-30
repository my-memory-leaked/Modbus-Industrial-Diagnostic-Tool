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
    logger->LogCritical(TAG, "Starting test...");
    _gui.SetProgressBar(0);
    // Create a QThread object
    QThread *testThread = new QThread(this);

    // Connect the thread's started signal to the slot where you'll run your test
    connect(testThread, &QThread::started, this, &FirmwareDetailVerification::executeTest);
    // connect(testThread, &QThread::finished, testThread, &QThread::deleteLater);

    // Start the thread
    testThread->start();

    (void)handleGUI();
    /* I don't have to delete because i pass the object to parent */
}

void FirmwareDetailVerification::testCompletedSuccessfully()
{
    logger->LogCritical(TAG, "Test successful!");
    _gui.SetProgressBar(100);

    QMessageBox::information(nullptr, "Test Success", "The firmware test was successful.");
}

void FirmwareDetailVerification::testFailed()
{
    logger->LogCritical(TAG, "Test failed!");
    _gui.SetProgressBar(100);

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


    QString language = getLanguage();

    if (language.isEmpty())
    {
        testFailed();
        return;
    }
    else
    {
        _gui.SetLanguageLabel(language);
    }
    _gui.SetProgressBar(55);


    auto serviceInterface = getServiceInterface();
    if (serviceInterface.first.isEmpty())
    {
        testFailed();
        return;
    }
    else
    {
        _gui.SetServiceInterfaceInfo(serviceInterface);
    }
    _gui.SetProgressBar(75);

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
    // data.setValueCount(9); // For debug purposes

    QModbusReply *firmwareReply = _mbStrategy->ReadData(data);
    if (!firmwareReply)
    {
        logger->LogCritical(TAG, "Failed to send read request for firmware version!");
        emit testFailed();
    }

    QEventLoop loop;
    connect(firmwareReply, &QModbusReply::finished, &loop, &QEventLoop::quit);
    QTimer::singleShot(2500, &loop, &QEventLoop::quit); // Timeout after 5000 ms
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
    quint8 lowerByte;
    quint8 upperByte;
    for (int i = 0; i < MAX_VERSION_LEN; ++i)
    {
        upperByte = payload[i] >> 8;
        lowerByte = payload[i] & 0xFF;

        firmwareVersion.append(QChar(upperByte));
        firmwareVersion.append(QChar(lowerByte));
    }

    return firmwareVersion.trimmed(); // Trim any whitespace
}

QString FirmwareDetailVerification::getLanguage()
{
    QString retVal;
    QModbusDataUnit data = _mbStrategy->GetQModbusDataUnitByName("Display");

    QModbusReply *languageReply = _mbStrategy->ReadData(data);
    if (!languageReply)
    {
        logger->LogCritical(TAG, "Failed to send read request for language!");
        emit testFailed();
        return QString();
    }

    QEventLoop loop;
    connect(languageReply, &QModbusReply::finished, &loop, &QEventLoop::quit);
    QTimer::singleShot(2500, &loop, &QEventLoop::quit); // Timeout after 5000 ms
    loop.exec();

    if (languageReply->isFinished())
    {
        if (languageReply->error() == QModbusDevice::NoError)
        {
            QString language = extractLanguage(languageReply);
            logger->LogDebug(TAG, "Language: " + language);
            retVal = language;
        }
        else
        {
            logger->LogCritical(TAG, "Failed to read language from device: " + languageReply->errorString());
            emit testFailed();
        }
    }
    else
    {
        logger->LogCritical(TAG, "Timeout waiting for language reply!");
        emit testFailed();
    }

    languageReply->deleteLater();
    return retVal;
}

QString FirmwareDetailVerification::extractLanguage(QModbusReply *languageReply)
{
    const QModbusDataUnit unit = languageReply->result();
    QVector<quint16> payload = unit.values();

    if (payload.size() == 0)
    {
        logger->LogCritical(TAG, "No data received for language!");
        emit testFailed();
        return QString();
    }

    QString language;

    switch(payload[0]) // Assuming payload[0] contains the language code
    {
        case 1: language = "Deutsch"; break;
        case 2: language = "English"; break;
        case 3: language = "Français"; break;
        case 4: language = "Español"; break;
        case 5: language = "Italiano"; break;
        case 6: language = "Pусский"; break;
        case 7: language = "Polski"; break;
        case 8: language = "Português"; break;
        case 9: language = "Türkçe"; break;
        case 10: language = "Magyar"; break;
        case 11: language = "中国"; break;
        case 12: language = "Nederlands"; break;
        case 13: language = "Čeština"; break;
        case 14: language = "Română"; break;
        case 15: language = "日本語"; break;
        case 16: language = "Български"; break;
        case 17: language = "Dansk"; break;
        case 18: language = "Ελληνικά"; break;
        case 19: language = "Suomi"; break;
        case 20: language = "한국어"; break;
        case 21: language = "Svenska"; break;
        case 22: language = "Tiếng Việt"; break;
        case 23: language = "عربي"; break;
        case 24: language = "Eesti keel"; break;
        case 25: language = "Hrvatski"; break;
        case 26: language = "Lietuvių"; break;
        case 27: language = "Latviešu"; break;
        case 28: language = "Norsk"; break;
        case 29: language = "Slovenčina"; break;
        case 30: language = "Slovenščina"; break;
        case 31: language = "Srpski"; break;
        case 32: language = "ไทย"; break;
        case 33: language = "Bahasa Indonesia"; break;
        case 34: language = "Bahasa Indonesia"; break; // Note: This is duplicated
        default:
            logger->LogCritical(TAG, "Unknown language code received!");
            emit testFailed();
    }

    return language;
}


std::pair<QString, int>FirmwareDetailVerification::getServiceInterface()
{
    std::pair<QString, int> retVal;

    QModbusDataUnit data = _mbStrategy->GetQModbusDataUnitByName("ServiceInterface");

    QModbusReply *serviceReply = _mbStrategy->ReadData(data);
    if (!serviceReply)
    {
        logger->LogCritical(TAG, "Failed to send read request for language!");
        emit testFailed();
        return std::pair<QString, int>();
    }

    QEventLoop loop;
    connect(serviceReply, &QModbusReply::finished, &loop, &QEventLoop::quit);
    QTimer::singleShot(2500, &loop, &QEventLoop::quit);
    loop.exec();

    if (serviceReply->isFinished())
    {
        if (serviceReply->error() == QModbusDevice::NoError)
        {
            std::pair<QString, int> result = parseServiceInterface(serviceReply);
            retVal = result;
        }
        else
        {
            logger->LogCritical(TAG, "Failed to read Service Interface");
            emit testFailed();
        }
    }
    else
    {
        logger->LogCritical(TAG, "Timeout waiting for Service Interface reply!");
        emit testFailed();
    }

    serviceReply->deleteLater();
    return retVal;
}

std::pair<QString, int>FirmwareDetailVerification::parseServiceInterface(QModbusReply *firmwareReply)
{
    constexpr uint8_t MAX_VERSION_LEN = 11;
    constexpr uint8_t DEVICE_ID_LEN = 10;
    const QModbusDataUnit unit = firmwareReply->result();
    QVector<quint16> payload = unit.values();

    if (payload.size() < MAX_VERSION_LEN) // Since each register is 2 bytes, 20 bytes are 10 registers
    {
        logger->LogCritical(TAG, "Incomplete data received for Service Interface!");
        testFailed();
        return std::pair<QString, int>();
    }

    std::pair<QString, int> result;
    quint8 upperByte;
    quint8 lowerByte;
    for (int i = 0; i < DEVICE_ID_LEN; ++i)
    {
        upperByte = payload[i] >> 8;
        lowerByte = payload[i] & 0xFF;

        result.first.append(QChar(upperByte));
        result.first.append(QChar(lowerByte));
    }

    result.second = payload[MAX_VERSION_LEN - 1];

    return result;
}

