#pragma once

#include <QModbusRtuSerialServer>
#include "ModbusStrategy.hpp"

class RTUStrategy : public ModbusStrategy
{
    Q_OBJECT

public:
    explicit RTUStrategy(const ModbusConnectionParameters &cConnectionParams, QObject *parent = nullptr);
    virtual ~RTUStrategy();

    virtual void Start() override;
    virtual void SendData(const QModbusDataUnit &cData) override;

private:
    QModbusRtuSerialServer* _modbusMaster;
};

