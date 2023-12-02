#include <JSONToDevicesConventer.hpp>
#include <Logger.hpp>
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <ModbusTCPClient.hpp>

static auto* logger = &Logger::GetInstance();

JSONToDevicesConventer::JSONToDevicesConventer() {}
JSONToDevicesConventer::~JSONToDevicesConventer() {}


std::list<std::shared_ptr<ModbusStrategy>> JSONToDevicesConventer::FromJson(const QByteArray &cJsonData)
{
    // Parse the JSON data
    QJsonDocument doc = QJsonDocument::fromJson(cJsonData);
    QJsonObject rootObj = doc.object();
    QJsonArray devicesArray = rootObj.value("Devices").toArray();

    std::list<std::shared_ptr<ModbusStrategy>> modbusStrategies;

    // Iterate through each device in the JSON array
    for (const QJsonValue &deviceVal : devicesArray)
    {
        QJsonObject deviceObj = deviceVal.toObject();
        QString deviceName = deviceObj.value("DeviceName").toString();

        // Check if the device has 'Parameters' or 'Registers' and handle accordingly
        QJsonArray paramsOrRegsArray = deviceObj.contains("Parameters") ?
                                           deviceObj.value("Parameters").toArray() :
                                           deviceObj.value("Registers").toArray();

        for (const QJsonValue &paramOrRegVal : paramsOrRegsArray)
        {
            QJsonObject paramOrRegObj = paramOrRegVal.toObject();

            logger->LogInfo(TAG, "Found Device: " + deviceName + " Type: " + paramOrRegObj.value("Type").toString());

            // Assuming a ModbusConnectionParameters object can be created from a QJsonObject
            ModbusConnectionParameters connectionParams(paramOrRegObj.value("IPAddress").toString(), paramOrRegObj.value("Port").toInt());

            // Create a ModbusStrategy object from the connection parameters
            std::shared_ptr<ModbusStrategy> strategy = std::make_shared<ModbusTCPClient>();
            strategy->SetDeviceName(deviceName);
            strategy->SetConnectionParameters(connectionParams);

            modbusStrategies.push_back(std::move(strategy));
        }
    }

    return modbusStrategies;
}


std::list<std::shared_ptr<ModbusStrategy>> JSONToDevicesConventer::FromJsonFile(const QString &cFilePath)
{
    QFile file(cFilePath);
    if (!file.open(QIODevice::ReadOnly))
    {
        logger->LogCritical(TAG, "Could not open file!!! " + cFilePath);
        return std::list<std::shared_ptr<ModbusStrategy>>();
    }

    QByteArray jsonData = file.readAll();
    file.close();

    return FromJson(jsonData);
}
