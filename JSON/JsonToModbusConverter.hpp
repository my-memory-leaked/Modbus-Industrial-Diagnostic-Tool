#pragma once
#include <QModbusDataUnit>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>

#include <Singleton.hpp>


class JsonToModbusConverter : public Singleton<JsonToModbusConverter>
{
    friend class Singleton<JsonToModbusConverter>;
public:
    QModbusDataUnitMap FromJson(const QByteArray &cJsonData);
    QModbusDataUnitMap FromJsonFile(const QString &cFilePath);

private:
    JsonToModbusConverter();
    ~JsonToModbusConverter();
    static constexpr const char TAG[] = "[JsonToModbusConverter]";
};
