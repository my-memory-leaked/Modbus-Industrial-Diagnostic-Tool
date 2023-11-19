#include <JSONToModbusRegisterConventer.hpp>
#include <Logger.hpp>
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <ModbusRegisterTypeMapper.hpp>

static auto* logger = &Logger::GetInstance();

JSONToModbusRegisterConventer::JSONToModbusRegisterConventer() {}

JSONToModbusRegisterConventer::~JSONToModbusRegisterConventer() {}


QVector<ModbusRegister> JSONToModbusRegisterConventer::FromJson(const QByteArray &cJsonData)
{
    QVector<ModbusRegister> registers;

    QJsonDocument doc = QJsonDocument::fromJson(cJsonData);
    QJsonObject rootObj = doc.object();
    QJsonArray devicesArray = rootObj["Devices"].toArray();

    for (const QJsonValue &deviceValue : devicesArray)
    {
        QJsonObject deviceObj = deviceValue.toObject();
        QString deviceName = deviceObj["DeviceName"].toString();

        logger->LogInfo(TAG, "Found device: " + deviceName);

        for (const QJsonValue &deviceValue : devicesArray)
        {
            QJsonObject deviceObj = deviceValue.toObject();
            QJsonArray registersArray = deviceObj["Registers"].toArray();

            for (const QJsonValue &registerValue : registersArray)
            {
                QJsonObject registerObj = registerValue.toObject();
                QString registerName = registerObj["RegisterName"].toString();
                QString registerTypeString = registerObj["RegisterType"].toString();
                int registerNumber = registerObj["RegisterNumber"].toInt();

                auto& regMapper = ModbusRegisterTypeMapper::GetInstance();
                QModbusDataUnit::RegisterType registerType = regMapper.StringToRegisterType(registerTypeString);

                registers.append(ModbusRegister(registerName, registerType, registerNumber));
            }
        }
    }

    return registers;
}


QVector<ModbusRegister> JSONToModbusRegisterConventer::FromJsonFile(const QString &cFilePath)
{
    QFile file(cFilePath);
    if (!file.open(QIODevice::ReadOnly))
    {
        logger->LogCritical(TAG, "Could not open file!!! " + cFilePath);
        return QVector<ModbusRegister>();
    }

    QByteArray jsonData = file.readAll();
    file.close();

    return FromJson(jsonData);
}

