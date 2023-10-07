#pragma once

#include <Singleton.hpp>
#include <QString>


class ApplicationConstant : private Singleton<ApplicationConstant>
{
    public:
    inline static QString MAIN_APPLICATION_NAME{"Modbus Industrial Diagnostic Tool"};



    private:
    friend class Singleton<ApplicationConstant>;
    ApplicationConstant() {}
    ~ApplicationConstant() {}
};

