#pragma once
#include <SystemResult.hpp>
#include <qstring.h>

class DeviceInterface
{
public:
    virtual SystemResult Connect() = 0;
    virtual SystemResult Disconnect() = 0;
    virtual bool IsConnected() const = 0;

    virtual QString GetDeviceName() const = 0;
    virtual quint32 GetDeviceID() const = 0;

    virtual ~DeviceInterface() {}

protected:
    bool _isConnected = false;
    QString _deviceName;
    quint32 _deviceID;
};
