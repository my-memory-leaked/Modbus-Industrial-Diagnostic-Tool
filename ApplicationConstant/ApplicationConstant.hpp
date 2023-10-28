#pragma once

#include <Singleton.hpp>
#include <QString>


class ApplicationConstant : private Singleton<ApplicationConstant>
{
    public:
    inline static QString APPLICATION_VERSION{"v0.01"};
    inline static QString MAIN_APPLICATION_NAME{"Modbus Industrial Diagnostic Tool"};
    inline static QString ADD_DEVICE_GUI_NAME{"Add device"};


    private:
    friend class Singleton<ApplicationConstant>;
    ApplicationConstant() {}
    ~ApplicationConstant() {}
};

