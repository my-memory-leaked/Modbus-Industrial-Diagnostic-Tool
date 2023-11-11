#include <ModbusRegister.hpp>

ModbusRegister::ModbusRegister() {}

ModbusRegister::ModbusRegister(const QString& cRegisterName, const QModbusDataUnit::RegisterType& cQModbusRegisterType, const int& cAddress)
    : _registerName(cRegisterName), _qModbusDataUnit(cQModbusRegisterType, cAddress, 1)
{
}

ModbusRegister::ModbusRegister(const QString& cRegisterName, const QModbusDataUnit& cQModbusDataUnit)
    : _registerName(cRegisterName), _qModbusDataUnit(cQModbusDataUnit)
{
}
ModbusRegister::ModbusRegister(const ModbusRegister& cModbusRegister)
    : _registerName(cModbusRegister._registerName), _qModbusDataUnit(cModbusRegister._qModbusDataUnit)
{
}

void ModbusRegister::SetName(const QString& cName)
{
    if(!cName.isEmpty())
        _registerName = cName;
}

void ModbusRegister::SetModbusDataUnit(const QModbusDataUnit& cQModbusDataUnit)
{
    if(cQModbusDataUnit.isValid())
        _qModbusDataUnit = cQModbusDataUnit;
}

QString ModbusRegister::GetName() const
{
    return _registerName;
}


bool ModbusRegister::operator==(const ModbusRegister& other) const
{
    return this->_registerName == other._registerName
           && this->_qModbusDataUnit.registerType() == other._qModbusDataUnit.registerType()
           && this->_qModbusDataUnit.startAddress() == other._qModbusDataUnit.startAddress()
           && this->_qModbusDataUnit.values() == other._qModbusDataUnit.values();
}
