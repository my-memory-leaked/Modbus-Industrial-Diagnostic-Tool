#pragma once
#include <QModbusDataUnit>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>

#include <Singleton.hpp>


class JsonToModbusMapConverter : public Singleton<JsonToModbusMapConverter>
{
    friend class Singleton<JsonToModbusMapConverter>;
public:
    QModbusDataUnitMap FromJson(const QByteArray &cJsonData);
    QModbusDataUnitMap FromJsonFile(const QString &cFilePath);

private:
    JsonToModbusMapConverter();
    ~JsonToModbusMapConverter();
    static constexpr const char TAG[] = "[JsonToModbusConverter]";
};
