#pragma once

#include <Singleton.hpp>
#include <QString>


class ApplicationConstant : private Singleton<ApplicationConstant>
{
    public:
    inline static QString APPLICATION_VERSION{"v0.01"};
    inline static QString MAIN_APPLICATION_NAME{"Modbus Industrial Diagnostic Tool"};
    inline static QString ADD_DEVICE_GUI_NAME{"Add device"};
    inline static QString CHANGE_DEVICE_SETTINGS_GUI_NAME{"Change device settings"};
    inline static QString AUMA_TEST_GUI_NAME{"Auma test"};

    inline static int TestCycles;

    private:
    friend class Singleton<ApplicationConstant>;
    ApplicationConstant() {}
    ~ApplicationConstant() {}
};

