#include <ActuatorResponseValidation.hpp>
#include <Logger.hpp>
#include <ErrorMask.hpp>
#include <WarningMask.hpp>
#include <QEventLoop>
#include <QTimer>
#include <QThread>
#include <QCoreApplication>
#include <QPair>
#include <QMessageBox>

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

    if (SystemResult::SYSTEM_OK == status && replay)
    {
        QEventLoop loop;
        connect(replay, &QModbusReply::finished, &loop, &QEventLoop::quit);
        QTimer::singleShot(2500, &loop, &QEventLoop::quit); // Timeout after 2500 ms
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
    logger->LogInfo(TAG, "Executing test...");
    SystemResult result = SystemResult::SYSTEM_OK;

    const float progressIncrement = 100.0f / 12; // 12 tasks in total
    float currentProgress = 0;

    for (quint16 i = 0; i < 3; ++i)
    {
        result = getActualPositionAndTorque();
        if (SystemResult::SYSTEM_OK != result)
            logger->LogCritical(TAG, "Getting actual position and torque failed!");

        currentProgress += progressIncrement;
        _gui.SetProgressBar(static_cast<int>(currentProgress));

        result = readErrors();
        if (SystemResult::SYSTEM_OK != result)
            logger->LogCritical(TAG, "Error read failed!");

        currentProgress += progressIncrement;
        _gui.SetProgressBar(static_cast<int>(currentProgress));

        result = readWarnings();
        if (SystemResult::SYSTEM_OK != result)
            logger->LogCritical(TAG, "Warnings read failed!");

        currentProgress += progressIncrement;
        _gui.SetProgressBar(static_cast<int>(currentProgress));

        (void) positionerTest();
        if (SystemResult::SYSTEM_OK != result)
            logger->LogCritical(TAG, "Positioner test failed!");

        currentProgress += progressIncrement;
        _gui.SetProgressBar(static_cast<int>(currentProgress));
    }

    testCompletedSuccessfully();
}

void ActuatorResponseValidation::handleGUI()
{
    _gui.exec();
}

void ActuatorResponseValidation::testCompletedSuccessfully()
{
    logger->LogInfo(TAG, "Test successful!");
    _gui.SetProgressBar(100);

    QMessageBox::information(nullptr, "Test Success", "The firmware test was successful.");
}

void ActuatorResponseValidation::testFailed()
{
    logger->LogCritical(TAG, "Test failed!");
    _gui.SetProgressBar(100);

    QMessageBox::critical(nullptr, "Test Failure", "The firmware test failed. Please check the logs for more information.");
}

SystemResult ActuatorResponseValidation::readWarnings()
{
    SystemResult retVal = SystemResult::SYSTEM_OK;
    QModbusDataUnit query(QModbusDataUnit::InputRegisters, 1008, 2);

    QModbusReply *reply = getMBReply(query);

    if (reply)
        retVal = parseWarnings(reply);

    if (SystemResult::SYSTEM_OK != retVal)
        logger->LogCritical(TAG, "Failed parsing warnings!");

    if (reply)
        reply->deleteLater();

    return retVal;
}

SystemResult ActuatorResponseValidation::parseWarnings(QModbusReply *reply)
{
    SystemResult retVal = SystemResult::SYSTEM_OK;

    if (!reply)
        retVal = SystemResult::SYSTEM_ERROR;

    if (reply->error() == QModbusDevice::NoError)
    {
        const QModbusDataUnit unit = reply->result();

        uint16_t unit1008 = unit.value(0);
        uint16_t unit1009 = unit.value(1);

        uint8_t highByteUnit1008 = unit1008 >> 8;
        uint8_t lowByteUnit1008 = unit1008 & 0xFF;

        uint8_t highByteUnit1009 = unit1009 >> 8;
        uint8_t lowByteUnit1009 = unit1009 & 0xFF;

        /* Parse 1008 register warnings */
        if (highByteUnit1008 & WarningMask::NO_REACTION)
        {
            logger->LogWarning(TAG, "No reaction warning occurred.");
            _gui.AddLog("No reaction warning occurred.");
            _warningCounter++;
        }
        if (highByteUnit1008 & WarningMask::SIL_FAULT)
        {
            logger->LogWarning(TAG, "SIL fault warning occurred.");
            _gui.AddLog("SIL fault warning occurred.");
            _warningCounter++;
        }
        if (highByteUnit1008 & WarningMask::TORQUE_WARN_OPEN)
        {
            logger->LogWarning(TAG, "Torque warn open warning occurred.");
            _gui.AddLog("Torque warn open warning occurred.");
            _warningCounter++;
        }
        if (highByteUnit1008 & WarningMask::TORQUE_WARN_CLOSE)
        {
            logger->LogWarning(TAG, "Torque warn close warning occurred.");
            _gui.AddLog("Torque warn close warning occurred.");
            _warningCounter++;
        }
        if (highByteUnit1008 & WarningMask::FQM_STATE_FAULT)
        {
            logger->LogWarning(TAG, "FQM state fault warning occurred.");
            _gui.AddLog("FQM state fault warning occurred.");
            _warningCounter++;
        }
        if (highByteUnit1008 & WarningMask::MAINTENANCE_REQUIRED)
        {
            logger->LogWarning(TAG, "Maintenance required warning occurred.");
            _gui.AddLog("Maintenance required warning occurred.");
            _warningCounter++;
        }

        if (lowByteUnit1008 & WarningMask::CONFIG_WARNING)
        {
            logger->LogWarning(TAG, "Configuration warning occurred.");
            _gui.AddLog("Configuration warning occurred.");
            _warningCounter++;
        }
        if (lowByteUnit1008 & WarningMask::RTC_NOT_SET)
        {
            logger->LogWarning(TAG, "RTC not set warning occurred.");
            _gui.AddLog("RTC not set warning occurred.");
            _warningCounter++;
        }
        if (lowByteUnit1008 & WarningMask::RTC_BUTTON_CELL)
        {
            logger->LogWarning(TAG, "RTC button cell warning occurred.");
            _gui.AddLog("RTC button cell warning occurred.");
            _warningCounter++;
        }
        if (lowByteUnit1008 & WarningMask::DC_EXTERNAL)
        {
            logger->LogWarning(TAG, "DC external warning occurred.");
            _gui.AddLog("DC external warning occurred.");
            _warningCounter++;
        }
        if (lowByteUnit1008 & WarningMask::CONTROLS_TEMP)
        {
            logger->LogWarning(TAG, "Controls temperature warning occurred.");
            _gui.AddLog("Controls temperature warning occurred.");
            _warningCounter++;
        }

        /* Parse 1009 register warnings */
        if (highByteUnit1009 & WarningMask::OP_TIME_WARNING)
        {
            logger->LogWarning(TAG, "Operation time warning occurred.");
            _gui.AddLog("Operation time warning occurred.");
            _warningCounter++;
        }
        if (highByteUnit1009 & WarningMask::WRM_RUNNING)
        {
            logger->LogWarning(TAG, "WRM running warning occurred.");
            _gui.AddLog("WRM running warning occurred.");
            _warningCounter++;
        }
        if (highByteUnit1009 & WarningMask::WRM_STARTS)
        {
            logger->LogWarning(TAG, "WRM starts warning occurred.");
            _gui.AddLog("WRM starts warning occurred.");
            _warningCounter++;
        }
        if (highByteUnit1009 & WarningMask::INTERNAL_WARNING)
        {
            logger->LogWarning(TAG, "Internal warning occurred.");
            _gui.AddLog("Internal warning occurred.");
            _warningCounter++;
        }
        if (highByteUnit1009 & WarningMask::INPUT_AIN1)
        {
            logger->LogWarning(TAG, "Input AIN1 warning occurred.");
            _gui.AddLog("Input AIN1 warning occurred.");
            _warningCounter++;
        }
        if (highByteUnit1009 & WarningMask::INPUT_AIN2)
        {
            logger->LogWarning(TAG, "Input AIN2 warning occurred.");
            _gui.AddLog("Input AIN2 warning occurred.");
            _warningCounter++;
        }
        if (highByteUnit1009 & WarningMask::WRM_FOC_CABLE_BUDGET)
        {
            logger->LogWarning(TAG, "WRM FOC cable budget warning occurred.");
            _gui.AddLog("WRM FOC cable budget warning occurred.");
            _warningCounter++;
        }

        if (lowByteUnit1009 & WarningMask::PVST_FAULT)
        {
            logger->LogWarning(TAG, "PVST fault warning occurred.");
            _gui.AddLog("PVST fault warning occurred.");
            _warningCounter++;
        }
        if (lowByteUnit1009 & WarningMask::PVST_ABORT)
        {
            logger->LogWarning(TAG, "PVST abort warning occurred.");
            _gui.AddLog("PVST abort warning occurred.");
            _warningCounter++;
        }
        if (lowByteUnit1009 & WarningMask::FAILURE_BEHAV_ACTIVE)
        {
            logger->LogWarning(TAG, "Failure behavior active warning occurred.");
            _gui.AddLog("Failure behavior active warning occurred.");
            _warningCounter++;
        }
        if (lowByteUnit1009 & WarningMask::FOC_CONNECTION_REQUIRED)
        {
            logger->LogWarning(TAG, "FOC connection required warning occurred.");
            _gui.AddLog("FOC connection required warning occurred.");
            _warningCounter++;
        }
        if (lowByteUnit1009 & WarningMask::PVST_REQUIRED)
        {
            logger->LogWarning(TAG, "PVST required warning occurred.");
            _gui.AddLog("PVST required warning occurred.");
            _warningCounter++;
        }
        if (lowByteUnit1009 & WarningMask::SETPOINT_POS)
        {
            logger->LogWarning(TAG, "Setpoint position warning occurred.");
            _gui.AddLog("Setpoint position warning occurred.");
            _warningCounter++;
        }

        _gui.SetWarningInfo(_warningCounter);
    }
    else
    {
        retVal = SystemResult::SYSTEM_ERROR;
        testFailed();
    }
    return retVal;
}

SystemResult ActuatorResponseValidation::readErrors()
{
    SystemResult retVal = SystemResult::SYSTEM_OK;
    QModbusDataUnit query(QModbusDataUnit::InputRegisters, 1007, 1);

    auto *reply1007 = getMBReply(query);

    if (reply1007)
        retVal = parseErrors(reply1007);

    if (retVal != SystemResult::SYSTEM_OK)
        logger->LogCritical(TAG, "Failed to parse errors!");

    if (reply1007)
        reply1007->deleteLater();

    return retVal;
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

        if (highByte & ErrorMask::NO_REACTION)
        {
            logger->LogCritical(TAG, "No reaction error occurred.");
            _gui.AddLog("No reaction error occurred.");
            _errorCounter++;
        }
        if (highByte & ErrorMask::INTERNAL_ERROR)
        {
            logger->LogCritical(TAG, "Internal error occurred.");
            _gui.AddLog("Internal error occurred.");
            _errorCounter++;
        }
        if (highByte & ErrorMask::TORQUE_FAULT_CLOSE)
        {
            logger->LogCritical(TAG, "Torque fault close error occurred.");
            _gui.AddLog("Torque fault close error occurred.");
            _errorCounter++;
        }
        if (highByte & ErrorMask::TORQUE_FAULT_OPEN)
        {
            logger->LogCritical(TAG, "Torque fault open error occurred.");
            _gui.AddLog("Torque fault open error occurred");
            _errorCounter++;
        }
        if (highByte & ErrorMask::PHASE_FAILURE)
        {
            logger->LogCritical(TAG, "Phase failure error occurred.");
            _gui.AddLog("Phase failure error occurred.");
            _errorCounter++;
        }
        if (highByte & ErrorMask::THERMAL_FAULT)
        {
            logger->LogCritical(TAG, "Thermal fault error occurred.");
            _gui.AddLog("Thermal fault error occurred.");
            _errorCounter++;
        }
        if (highByte & ErrorMask::MAINS_FAULT)
        {
            logger->LogCritical(TAG, "Mains fault error occurred.");
            _gui.AddLog("Mains fault error occurred.");
            _errorCounter++;
        }
        if (highByte & ErrorMask::CONFIGURATION_ERROR)
        {
            logger->LogCritical(TAG, "Configuration error occurred.");
            _gui.AddLog("Configuration error occurred.");
            _errorCounter++;
        }

        // Check for Fault 2 errors
        if (lowByte & ErrorMask::INCORRECT_PHASE_SEQ)
        {
            logger->LogCritical(TAG, "Incorrect phase sequence error occurred.");
            _gui.AddLog("Incorrect phase sequence error occurred.");
            _errorCounter++;
        }
        if (lowByte & ErrorMask::CONT_ERROR_REMOTE)
        {
            logger->LogCritical(TAG, "Continuous error remote occurred.");
            _gui.AddLog("Continuous error remote occurred.");
            _errorCounter++;
        }
        if (lowByte & ErrorMask::INCORRECT_ROTATION)
        {
            logger->LogCritical(TAG, "Incorrect rotation error occurred.");
            _gui.AddLog("Incorrect rotation error occurred.");
            _errorCounter++;
        }

        _gui.SetErrorInfo(_errorCounter);
    }
    else
    {
        retVal = SystemResult::SYSTEM_ERROR;
        testFailed();
    }
    return retVal;
}

void ActuatorResponseValidation::positionerTest()
{
    // Otworz do 100
    fullyOpenAndCheckPosition();

    fullyCloseAndCheckPosition();
    getActualPositionAndTorque();
}

SystemResult ActuatorResponseValidation::getActualPositionAndTorque()
{
    SystemResult retVal = SystemResult::SYSTEM_OK;

    QModbusDataUnit query = _mbStrategy->GetQModbusDataUnitByName("General");
    QModbusReply *reply = getMBReply(query);

    if(reply)
    {
        const QModbusDataUnit unit = reply->result();
        QPair<QString, QString> posNTorq = parsePositionAndTorque(unit);
        logger->LogDebug(TAG, "Read position: " + posNTorq.first + " torque: " + posNTorq.second);

        _gui.SetActualPosition(posNTorq.first);
        _gui.SetActualTorque(posNTorq.second);
    }
    else
    {
        logger->LogCritical(TAG, "Failed to read position and torque from device: " + reply->errorString());
        retVal = SystemResult::SYSTEM_ERROR;
        testFailed();
    }

    reply->deleteLater();

    return retVal;
}

SystemResult ActuatorResponseValidation::fullyOpenAndCheckPosition()
{
    SystemResult retVal = SystemResult::SYSTEM_OK;
    _gui.SetActuatorRunningDiode(true);
    retVal = fieldbusOpen(true);

    if (retVal != SystemResult::SYSTEM_OK)
        logger->LogCritical(TAG, "Failed to open fieldbus!");


    // Create a QTimer
    QTimer timer;
    timer.setInterval(1500); // 1.5 seconds

    // Connect the QTimer's timeout signal to getActualPositionAndTorque
    connect(&timer, &QTimer::timeout, this, &ActuatorResponseValidation::getActualPositionAndTorque);

    // Start the timer
    timer.start();

    QEventLoop loop;
    QTimer::singleShot(15000, &loop, &QEventLoop::quit);
    loop.exec();

    // Stop the timer after exiting the loop
    timer.stop();

    if(!checkIfPositionReached(100))
    {
        logger->LogCritical(TAG, "Failed to reach assigned position!");
        retVal = SystemResult::SYSTEM_ERROR;
    }

    retVal = fieldbusOpen(false);
    _gui.SetActuatorRunningDiode(false);
    return retVal;
}
SystemResult ActuatorResponseValidation::fullyCloseAndCheckPosition()
{
    SystemResult retVal = SystemResult::SYSTEM_OK;

    _gui.SetActuatorRunningDiode(true);
    retVal = fieldbusClose(true);

    if (retVal != SystemResult::SYSTEM_OK)
        logger->LogCritical(TAG, "Failed to open fieldbus!");


    // Create a QTimer
    QTimer timer;
    timer.setInterval(1500); // 1.5 seconds

    // Connect the QTimer's timeout signal to getActualPositionAndTorque
    connect(&timer, &QTimer::timeout, this, &ActuatorResponseValidation::getActualPositionAndTorque);

    // Start the timer
    timer.start();

    QEventLoop loop;
    QTimer::singleShot(15000, &loop, &QEventLoop::quit);
    loop.exec();

    // Stop the timer after exiting the loop
    timer.stop();

    if(!checkIfPositionReached(0))
    {
        logger->LogCritical(TAG, "Failed to reach assigned position!");
        retVal = SystemResult::SYSTEM_ERROR;
    }

    retVal = fieldbusClose(false);
    _gui.SetActuatorRunningDiode(false);

    return retVal;
}

bool ActuatorResponseValidation::checkIfPositionReached(int position)
{
    bool retVal = false;
    QModbusDataUnit query = _mbStrategy->GetQModbusDataUnitByName("General");
    QModbusReply *reply = getMBReply(query);

    if(reply)
    {
        const QModbusDataUnit unit = reply->result();
        auto posNTorq = parsePositionAndTorqueInt(unit);

        if(posNTorq.first == position)
        {
            retVal = true;
            logger->LogDebug(TAG, "Position reached!");
        }
    }
    else
    {
        logger->LogCritical(TAG, "Failed to read position and torque from device: " + reply->errorString());
    }
    reply->deleteLater();
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
    constexpr uint8_t MINIMAL_LENGTH = 5;
    constexpr uint8_t POSITION_SHIFT = 3;
    constexpr uint8_t TORQUE_SHIFT = 4;

    QVector<quint16> payload = unit.values();
    QPair<QString, QString> retVal;

    if (payload.size() < MINIMAL_LENGTH) // Since each register is 2 bytes, 20 bytes are 10 registers
    {
        logger->LogCritical(TAG, "Incomplete data received for Service Interface!");
        testFailed();
        return QPair<QString, QString>();
    }

    quint32 position = payload[POSITION_SHIFT] / 10;
    retVal.first.append(QString::number(position) + " %");

    quint32 torque = payload[TORQUE_SHIFT] / 10;
    retVal.second.append(QString::number(torque) + " Nm");

    return retVal;
}

QPair<int, int> ActuatorResponseValidation::parsePositionAndTorqueInt(const QModbusDataUnit &unit)
{
    constexpr uint8_t MINIMAL_LENGTH = 5;
    constexpr uint8_t POSITION_SHIFT = 3;
    constexpr uint8_t TORQUE_SHIFT = 4;

    QVector<quint16> payload = unit.values();
    QPair<int, int> retVal;

    if (payload.size() < MINIMAL_LENGTH) // Since each register is 2 bytes, 20 bytes are 10 registers
    {
        logger->LogCritical(TAG, "Incomplete data received for Service Interface!");
        testFailed();
        return QPair<int, int>();
    }

    int position = payload[POSITION_SHIFT] / 10;
    retVal.first = position;

    int torque = payload[TORQUE_SHIFT] / 10;
    retVal.second = torque;

    _gui.SetActualPosition(QString::number(position) + " %");
    _gui.SetActualTorque(QString::number(torque) + " Nm");

    return retVal;
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

SystemResult ActuatorResponseValidation::fieldbusOpen(bool state)
{
    SystemResult retVal = SystemResult::SYSTEM_OK;
    QModbusDataUnit writeUnit(QModbusDataUnit::Coils, 0, 1);
    writeUnit.setValue(0, state ? 0xFF00 : 0x0000);

    _mbStrategy->WriteData(writeUnit);
    return retVal;
}

SystemResult ActuatorResponseValidation::fieldbusClose(bool state)
{
    SystemResult retVal = SystemResult::SYSTEM_OK;
    QModbusDataUnit writeUnit(QModbusDataUnit::Coils, 1, 1);
    writeUnit.setValue(0,  state ? 0xFF00 : 0x0000);

    _mbStrategy->WriteData(writeUnit);

    return retVal;
}
