#include <ActuatorResponseValidation.hpp>
#include <Logger.hpp>
#include <QEventLoop>
#include <QTimer>

static auto* logger = &Logger::GetInstance();

ActuatorResponseValidation::ActuatorResponseValidation() {}
ActuatorResponseValidation::~ActuatorResponseValidation() {}

void ActuatorResponseValidation::RunTest()
{

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

