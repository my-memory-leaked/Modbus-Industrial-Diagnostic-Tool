#include "ModbusException.hpp"

ModbusException::ModbusException(const QString& cErrorMessage) : _errorMessage(cErrorMessage) {}

void ModbusException::raise() const
{
    throw *this;
}
QException *ModbusException::clone() const
{
    return new ModbusException(*this);
}

const char * ModbusException::what() const noexcept
{
    return _errorMessage.toStdString().c_str();
}
