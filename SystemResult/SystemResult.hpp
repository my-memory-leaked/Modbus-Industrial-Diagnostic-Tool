#pragma once
#include <QtGlobal>

enum class SystemResult : qint8
{
    SYSTEM_ERROR = -1,
    SYSTEM_OK = 0,
    SYSTEM_TIMEOUT = 2,
    SYSTEM_INVALID_ARGUMENT,
    SYSTEM_INVALID_SIZE,
    SYSTEM_NOT_FOUND
};
