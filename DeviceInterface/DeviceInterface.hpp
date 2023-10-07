#pragma once
#include <SystemResult.hpp>

class DeviceInterface
{
public:
    virtual SystemResult Connect() = 0;
    virtual SystemResult Disconnect() = 0;
    virtual bool IsConnected() const = 0;

    virtual ~DeviceInterface() {}

protected:
    bool _isConnected = false;
};
