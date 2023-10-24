#pragma once

#include <QObject>
#include <Singleton.hpp>

class ModbusParser : public QObject, public Singleton<ModbusParser>
{
    Q_OBJECT
    friend class Singleton<ModbusParser>;
public slots:



private:
    ModbusParser();
    ~ModbusParser();
};
