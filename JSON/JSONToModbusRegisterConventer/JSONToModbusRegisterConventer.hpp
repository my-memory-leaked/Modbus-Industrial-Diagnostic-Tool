#pragma once
#include <Singleton.hpp>
#include <QVector>
#include <ModbusRegister.hpp>


class JSONToModbusRegisterConventer : public Singleton<JSONToModbusRegisterConventer>
{
    friend class Singleton<JSONToModbusRegisterConventer>;
public:

    QVector<ModbusRegister> FromJson(const QByteArray &cJsonData);
    QVector<ModbusRegister> FromJsonFile(const QString &cFilePath);

private:
    JSONToModbusRegisterConventer();
    ~JSONToModbusRegisterConventer();

    static constexpr const char TAG[] = "[JSONToModbusRegisterConventer]";
};
