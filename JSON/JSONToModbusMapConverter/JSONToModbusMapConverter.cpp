#include <JSONToModbusMapConverter.hpp>
#include <ModbusRegisterTypeMapper.hpp>
#include <Logger.hpp>

static auto* logger = &Singleton<Logger>::GetInstance();

JsonToModbusMapConverter::JsonToModbusMapConverter() {}

JsonToModbusMapConverter::~JsonToModbusMapConverter() {}

QModbusDataUnitMap JsonToModbusMapConverter::FromJson(const QByteArray &cJsonData)
{
    QModbusDataUnitMap resultMap;

    QJsonDocument doc = QJsonDocument::fromJson(cJsonData);
    QJsonObject root = doc.object();

    for (auto it = root.begin(); it != root.end(); ++it)
    {
        QModbusDataUnit::RegisterType registerType = ModbusRegisterTypeMapper::StringToRegisterType(it.key());

        QJsonArray dataArray = it.value().toArray();
        QModbusDataUnit dataUnit(registerType);

        // Reset start address for each register type to maximum, this will be adjusted
        int startAddress = INT_MAX;

        for (const QJsonValue &val : dataArray)
        {
            QJsonObject dataObj = val.toObject();
            int address = dataObj["address"].toInt() - 1;  // Subtracting 1 to handle Modbus 0-based internal addressing
            int value = dataObj["value"].toInt();

            // Updating start address for the data unit if needed
            if(address < startAddress)
            {
                startAddress = address;
            }

            dataUnit.setValue(address, value);  // Assign value to the corresponding address
        }

        // Setting the correct start address and count for the QModbusDataUnit
        dataUnit.setStartAddress(startAddress);
        dataUnit.setValueCount(dataArray.size());

        resultMap.insert(registerType, dataUnit);
    }

    return resultMap;
}

QModbusDataUnitMap JsonToModbusMapConverter::FromJsonFile(const QString &cFilePath)
{
    QFile file(cFilePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return QModbusDataUnitMap();

    QByteArray jsonData = file.readAll();
    return FromJson(jsonData);
}
