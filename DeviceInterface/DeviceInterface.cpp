#include <DeviceInterface.hpp>


DeviceInterface::DeviceInterface()
{
    static quint32 deviceID = -1;
    _deviceID = ++deviceID;
    _deviceName = "DefaultInterface";
}

DeviceInterface::~DeviceInterface() {}

bool DeviceInterface::IsConnected() const
{
    return _isConnected;
}

void DeviceInterface::SetDeviceName(const QString& newName)
{
    if(!newName.isEmpty())
        _deviceName = newName;
}


QString DeviceInterface::GetDeviceName() const
{
    return _deviceName;
}

quint32 DeviceInterface::GetDeviceID() const
{
    return _deviceID;
}
