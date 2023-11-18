#pragma once
#include <QObject>
#include <SystemResult.hpp>
#include <qstring.h>

class DeviceInterface : public QObject
{
    Q_OBJECT
public:
    virtual SystemResult Connect() = 0;
    virtual SystemResult Disconnect() = 0;
    virtual bool IsConnected() const;

    virtual void SetDeviceName(const QString& newName);
    virtual QString GetDeviceName() const;
    virtual quint32 GetDeviceID() const;

    DeviceInterface();
    virtual ~DeviceInterface();

protected:
    bool _isConnected;
    QString _deviceName;
    quint32 _deviceID;
};
