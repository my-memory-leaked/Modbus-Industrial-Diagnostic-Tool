#include <ActuatorResponseValidation.hpp>
#include <Logger.hpp>
#include <ErrorMask.hpp>
#include <WarningMask.hpp>
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

    if (SystemResult::SYSTEM_OK == status && replay)
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
    logger->LogInfo(TAG, "Executing test...");
    SystemResult result = SystemResult::SYSTEM_OK;
    // Otworzyć i zamknąć 2 razy
    // 3 setpointy randomowe, sprawdzanie coilsów oraz statusów błędów, wyświetlanie diod dostępnych na urządzeniu

    // Stany do czytania: 1000 coil 5 bajt: 7 bit , 6 bit, 2 ,1 || 6 bajt 4 bit
    // 1002
    // for (quint16 i = 0; i < 3; ++i)
    // {
    //Gotowe
        // result = getActualPositionAndTorque();
        // if (SystemResult::SYSTEM_OK != result)
        //     logger->LogCritical(TAG, "Getting actual position and torque failed!");

        // result = readErrors();
        // if (SystemResult::SYSTEM_OK != result)
        //     logger->LogCritical(TAG, "Error read failed!");


    // TODO
        result = readWarnings();
        if (SystemResult::SYSTEM_OK != result)
            logger->LogCritical(TAG, "Warnings read failed!");


        // result = positionerTest();
        // if (SystemResult::SYSTEM_OK != result)
        //     logger->LogCritical(TAG, "Positioner test failed!");
    // }
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
            logger->LogWarning(TAG, "No reaction warning occurred.");
        if (highByteUnit1008 & WarningMask::SIL_FAULT)
            logger->LogWarning(TAG, "SIL fault warning occurred.");
        if (highByteUnit1008 & WarningMask::TORQUE_WARN_OPEN)
            logger->LogWarning(TAG, "Torque warn open warning occurred.");
        if (highByteUnit1008 & WarningMask::TORQUE_WARN_CLOSE)
            logger->LogWarning(TAG, "Torque warn close warning occurred.");
        if (highByteUnit1008 & WarningMask::FQM_STATE_FAULT)
            logger->LogWarning(TAG, "FQM state fault warning occurred.");
        if (highByteUnit1008 & WarningMask::MAINTENANCE_REQUIRED)
            logger->LogWarning(TAG, "Maintenance required warning occurred.");

        if (lowByteUnit1008 & WarningMask::CONFIG_WARNING)
            logger->LogWarning(TAG, "Configuration warning occurred.");
        if (lowByteUnit1008 & WarningMask::RTC_NOT_SET)
            logger->LogWarning(TAG, "RTC not set warning occurred.");
        if (lowByteUnit1008 & WarningMask::RTC_BUTTON_CELL)
            logger->LogWarning(TAG, "RTC button cell warning occurred.");
        if (lowByteUnit1008 & WarningMask::DC_EXTERNAL)
            logger->LogWarning(TAG, "DC external warning occurred.");
        if (lowByteUnit1008 & WarningMask::CONTROLS_TEMP)
            logger->LogWarning(TAG, "Controls temperature warning occurred.");

        /* Parse 1009 register warnings */
        if (highByteUnit1009 & WarningMask::OP_TIME_WARNING)
            logger->LogWarning(TAG, "Operation time warning occurred.");
        if (highByteUnit1009 & WarningMask::WRM_RUNNING)
            logger->LogWarning(TAG, "WRM running warning occurred.");
        if (highByteUnit1009 & WarningMask::WRM_STARTS)
            logger->LogWarning(TAG, "WRM starts warning occurred.");
        if (highByteUnit1009 & WarningMask::INTERNAL_WARNING)
            logger->LogWarning(TAG, "Internal warning occurred.");
        if (highByteUnit1009 & WarningMask::INPUT_AIN1)
            logger->LogWarning(TAG, "Input AIN1 warning occurred.");
        if (highByteUnit1009 & WarningMask::INPUT_AIN2)
            logger->LogWarning(TAG, "Input AIN2 warning occurred.");
        if (highByteUnit1009 & WarningMask::WRM_FOC_CABLE_BUDGET)
            logger->LogWarning(TAG, "WRM FOC cable budget warning occurred.");

        if (lowByteUnit1009 & WarningMask::PVST_FAULT)
            logger->LogWarning(TAG, "PVST fault warning occurred.");
        if (lowByteUnit1009 & WarningMask::PVST_ABORT)
            logger->LogWarning(TAG, "PVST abort warning occurred.");
        if (lowByteUnit1009 & WarningMask::FAILURE_BEHAV_ACTIVE)
            logger->LogWarning(TAG, "Failure behavior active warning occurred.");
        if (lowByteUnit1009 & WarningMask::FOC_CONNECTION_REQUIRED)
            logger->LogWarning(TAG, "FOC connection required warning occurred.");
        if (lowByteUnit1009 & WarningMask::PVST_REQUIRED)
            logger->LogWarning(TAG, "PVST required warning occurred.");
        if (lowByteUnit1009 & WarningMask::SETPOINT_POS)
            logger->LogWarning(TAG, "Setpoint position warning occurred.");
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
            logger->LogCritical(TAG, "No reaction error occurred.");
        if (highByte & ErrorMask::INTERNAL_ERROR)
            logger->LogCritical(TAG, "Internal error occurred.");
        if (highByte & ErrorMask::TORQUE_FAULT_CLOSE)
            logger->LogCritical(TAG, "Torque fault close error occurred.");
        if (highByte & ErrorMask::TORQUE_FAULT_OPEN)
            logger->LogCritical(TAG, "Torque fault open error occurred.");
        if (highByte & ErrorMask::PHASE_FAILURE)
            logger->LogCritical(TAG, "Phase failure error occurred.");
        if (highByte & ErrorMask::THERMAL_FAULT)
            logger->LogCritical(TAG, "Thermal fault error occurred.");
        if (highByte & ErrorMask::MAINS_FAULT)
            logger->LogCritical(TAG, "Mains fault error occurred.");
        if (highByte & ErrorMask::CONFIGURATION_ERROR)
            logger->LogCritical(TAG, "Configuration error occurred.");

        // Check for Fault 2 errors
        if (lowByte & ErrorMask::INCORRECT_PHASE_SEQ)
            logger->LogCritical(TAG, "Incorrect phase sequence error occurred.");
        if (lowByte & ErrorMask::CONT_ERROR_REMOTE)
            logger->LogCritical(TAG, "Continuous error remote occurred.");
        if (lowByte & ErrorMask::INCORRECT_ROTATION)
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
