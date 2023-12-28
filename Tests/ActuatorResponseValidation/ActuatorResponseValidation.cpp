#include <ActuatorResponseValidation.hpp>
#include <Logger.hpp>
#include <ErrorMask.hpp>
#include <QEventLoop>
#include <QTimer>
#include <QThread>
#include <QCoreApplication>
#include <QPair>

static auto *logger = &Logger::GetInstance();

ActuatorResponseValidation::ActuatorResponseValidation()
{
    _mbController = &ModbusController::GetInstance();
    if (!_mbController)
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

ActuatorResponseValidation::~ActuatorResponseValidation() {}

void ActuatorResponseValidation::RunTest()
{
    logger->LogInfo(TAG, "Starting test...");
    _gui.SetProgressBar(0);
    // Create a QThread object
    QThread *testThread = new QThread(this);

    // Connect the thread's started signal to the slot where you'll run your test
    connect(testThread, &QThread::started, this, &ActuatorResponseValidation::executeTest);
    // connect(testThread, &QThread::finished, testThread, &QThread::deleteLater);

    // Start the thread
    testThread->start();

    (void)handleGUI();
    /* I don't have to delete because i pass the object to parent */
}

QModbusReply *ActuatorResponseValidation::getMBReply(const QModbusDataUnit query)
{
    SystemResult status = SystemResult::SYSTEM_OK;
    QModbusReply *replay = _mbStrategy->ReadData(query);

    if (!replay)
    {
        logger->LogCritical(TAG, "Failed to send read request for warnings!");
        status = SystemResult::SYSTEM_ERROR;
        testFailed();
    }

    if (SystemResult::SYSTEM_OK != status && replay)
    {
        QEventLoop loop;
        connect(replay, &QModbusReply::finished, &loop, &QEventLoop::quit);
        QTimer::singleShot(2500, &loop, &QEventLoop::quit); // Timeout after 5000 ms
        loop.exec();

        if (replay->isFinished())
        {
            if (replay->error() != QModbusDevice::NoError)
            {
                status = SystemResult::SYSTEM_ERROR;
                testFailed();
            }
        }
        else
        {
            logger->LogCritical(TAG, "Timeout waiting for reply!");
            status = SystemResult::SYSTEM_ERROR;
            testFailed();
        }
    }

    return replay;
}

void ActuatorResponseValidation::executeTest()
{
    bool isTestFinished = false;
    SystemResult result = SystemResult::SYSTEM_OK;
    // Otworzyć i zamknąć 2 razy
    // 3 setpointy randomowe, sprawdzanie coilsów oraz statusów błędów, wyświetlanie diod dostępnych na urządzeniu

    // Stany do czytania: 1000 coil 5 bajt: 7 bit , 6 bit, 2 ,1 || 6 bajt 4 bit
    // 1002
    for (quint16 i = 0; i < 3; ++i)
    {
        result = getActualPositionAndTorque();
        if (SystemResult::SYSTEM_OK != result)
            logger->LogCritical(TAG, "Getting actual position and torque failed!");

        result = readWarnings();
        if (SystemResult::SYSTEM_OK != result)
            logger->LogCritical(TAG, "Warnings read failed!");

        result = readErrors();
        if (SystemResult::SYSTEM_OK != result)
            logger->LogCritical(TAG, "Error read failed!");

        result = positionerTest();
        if (SystemResult::SYSTEM_OK != result)
            logger->LogCritical(TAG, "Positioner test failed!");
    }
}

void ActuatorResponseValidation::handleGUI()
{
    _gui.exec();
}

void ActuatorResponseValidation::testCompletedSuccessfully()
{
}

void ActuatorResponseValidation::testFailed()
{
}

void ActuatorResponseValidation::toggleWarningDiode()
{
    static bool isToggled = false;

    isToggled ? _gui.SetWarningDiode(true) : _gui.SetWarningDiode(false);

    isToggled = !isToggled;
}

void ActuatorResponseValidation::toggleErrorDiode()
{
    static bool isToggled = false;

    isToggled ? _gui.SetErrorDiode(true) : _gui.SetErrorDiode(false);

    isToggled = !isToggled;
}

SystemResult ActuatorResponseValidation::readWarnings()
{
    SystemResult retVal = SystemResult::SYSTEM_OK;
    QModbusDataUnit query(QModbusDataUnit::Coils, 1008, 1);

    QModbusReply *reply1008 = getMBReply(query);

    if (reply1008)
        retVal = parseWarnings(reply1008);

    if (SystemResult::SYSTEM_OK != retVal)
        logger->LogCritical(TAG, "Failed parsing warnings!");

    (void)query.setStartAddress(1009);

    auto *reply1009 = getMBReply(query);

    if (reply1009)
        retVal = parseWarnings(reply1009);

    if (SystemResult::SYSTEM_OK != retVal)
        logger->LogCritical(TAG, "Failed parsing warnings!");

    if (reply1008)
        reply1008->deleteLater();

    if (reply1009)
        reply1009->deleteLater();

    return retVal;
}

SystemResult ActuatorResponseValidation::parseWarnings(QModbusReply *reply)
{
}

SystemResult ActuatorResponseValidation::readErrors()
{
    SystemResult retVal = SystemResult::SYSTEM_OK;
    QModbusDataUnit query(QModbusDataUnit::Coils, 1007, 1);

    auto *reply1007 = getMBReply(query);

    if (reply1007)
        retVal = parseErrors(reply1007);

    if (retVal != SystemResult::SYSTEM_OK)
        logger->LogCritical(TAG, "Failed to parse errors!");

    if (reply1007)
        reply1007->deleteLater();
}

SystemResult ActuatorResponseValidation::parseErrors(QModbusReply *reply)
{
    SystemResult retVal = SystemResult::SYSTEM_OK;

    if (!reply)
        retVal = SystemResult::SYSTEM_ERROR;

    if (reply->error() == QModbusDevice::NoError)
    {
        const QModbusDataUnit unit = reply->result();

        uint16_t value = unit.value(0);

        // High byte (Fault 1)
        uint8_t highByte = value >> 8;
        // Low byte (Fault 2)
        uint8_t lowByte = value & 0xFF;

        if (lowByte & ErrorMask::NO_REACTION)
            logger->LogCritical(TAG, "No reaction error occurred.");
        if (lowByte & ErrorMask::INTERNAL_ERROR)
            logger->LogCritical(TAG, "Internal error occurred.");
        if (lowByte & ErrorMask::TORQUE_FAULT_CLOSE)
            logger->LogCritical(TAG, "Torque fault close error occurred.");
        if (lowByte & ErrorMask::TORQUE_FAULT_OPEN)
            logger->LogCritical(TAG, "Torque fault open error occurred.");
        if (lowByte & ErrorMask::PHASE_FAILURE)
            logger->LogCritical(TAG, "Phase failure error occurred.");
        if (lowByte & ErrorMask::THERMAL_FAULT)
            logger->LogCritical(TAG, "Thermal fault error occurred.");
        if (lowByte & ErrorMask::MAINS_FAULT)
            logger->LogCritical(TAG, "Mains fault error occurred.");
        if (lowByte & ErrorMask::CONFIGURATION_ERROR)
            logger->LogCritical(TAG, "Configuration error occurred.");

        // Check for Fault 2 errors
        if (highByte & ErrorMask::INCORRECT_PHASE_SEQ)
            logger->LogCritical(TAG, "Incorrect phase sequence error occurred.");
        if (highByte & ErrorMask::CONT_ERROR_REMOTE)
            logger->LogCritical(TAG, "Continuous error remote occurred.");
        if (highByte & ErrorMask::INCORRECT_ROTATION)
            logger->LogCritical(TAG, "Incorrect rotation error occurred.");
    }
    else
    {
        retVal = SystemResult::SYSTEM_ERROR;
        testFailed();
    }
    return retVal;
}

SystemResult ActuatorResponseValidation::positionerTest()
{
}

SystemResult ActuatorResponseValidation::checkPositionerRunning()
{
}

SystemResult ActuatorResponseValidation::setPositioner(quint32 position)
{

    // Set Fieldbus SETPOINT
    // Byte commands
}

SystemResult ActuatorResponseValidation::resetPositioner(quint32 position)
{
    // Set Fieldbus RESET
    //
}

SystemResult ActuatorResponseValidation::getActualPositionAndTorque()
{
    SystemResult retVal = SystemResult::SYSTEM_OK;

    QModbusDataUnit data = _mbStrategy->GetQModbusDataUnitByName("General");

    QModbusReply *firmwareReply = _mbStrategy->ReadData(data);
    if (!firmwareReply)
    {
        logger->LogCritical(TAG, "Failed to send read request for actual position and torque version!");
        retVal = SystemResult::SYSTEM_ERROR;
        testFailed();
    }

    if (SystemResult::SYSTEM_OK != retVal && firmwareReply)
    {
        QEventLoop loop;
        connect(firmwareReply, &QModbusReply::finished, &loop, &QEventLoop::quit);
        QTimer::singleShot(2500, &loop, &QEventLoop::quit); // Timeout after 5000 ms
        loop.exec();

        if (firmwareReply->isFinished())
        {
            if (firmwareReply->error() == QModbusDevice::NoError)
            {
                const QModbusDataUnit unit = firmwareReply->result();
                QPair<QString, QString> posNTorq = parsePositionAndTorque(unit);
                logger->LogDebug(TAG, "Read position: " + posNTorq.first + " torque: " + posNTorq.second);
            }
            else
            {
                logger->LogCritical(TAG, "Failed to read position and torque from device: " + firmwareReply->errorString());
                retVal = SystemResult::SYSTEM_ERROR;
                testFailed();
            }
        }
        else
        {
            logger->LogCritical(TAG, "Timeout waiting for postion and torque reply!");
            retVal = SystemResult::SYSTEM_ERROR;
            testFailed();
        }

        firmwareReply->deleteLater();
    }

    return retVal;
}

SystemResult ActuatorResponseValidation::testOpenTo80Percent(ModbusStrategy *mbStrategy)
{
    return testActuatorPositioning(mbStrategy, 800); // 80% of a scale of 1000
}

SystemResult ActuatorResponseValidation::testActuatorPositioning(ModbusStrategy *mbStrategy, int targetPosition)
{
    SystemResult retVal = SystemResult::SYSTEM_OK;

    // Get the Modbus data unit for setting the actuator position
    QModbusDataUnit setPositionUnit = mbStrategy->GetQModbusDataUnitByName("Positioner");

    // Prepare a data unit for reading the actual position
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

int ActuatorResponseValidation::extractPositionValue(QModbusReply *reply)
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

bool ActuatorResponseValidation::positionReached(int actualPosition, int targetPosition, int tolerance)
{
    return std::abs(actualPosition - targetPosition) <= tolerance;
}

QPair<QString, QString> ActuatorResponseValidation::parsePositionAndTorque(const QModbusDataUnit &unit)
{
}

QString ActuatorResponseValidation::modbusDataUnitToString(const QModbusDataUnit &unit)
{
    QString result;
    for (uint i = 0; i < unit.valueCount(); ++i)
    {
        quint16 value = unit.value(i);
        result.append(QChar(value));
    }
    return result;
}

SystemResult ActuatorResponseValidation::_FieldbusOpen(ModbusStrategy *mbStrategy, bool state)
{
    SystemResult retVal = SystemResult::SYSTEM_OK;
    QModbusDataUnit query(QModbusDataUnit::Coils, 0, state);
    mbStrategy->WriteData(query);
    return retVal;
}

SystemResult ActuatorResponseValidation::_FieldbusClose(ModbusStrategy *mbStrategy, bool state)
{
    SystemResult retVal = SystemResult::SYSTEM_OK;
    QModbusDataUnit query(QModbusDataUnit::Coils, 1, state);
    mbStrategy->WriteData(query);

    return retVal;
}
