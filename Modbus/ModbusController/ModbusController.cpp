#include <ModbusController.hpp>
#include <Logger.hpp>
#include <ModbusTCPClient.hpp>


static auto* logger = &Singleton<Logger>::GetInstance();

ModbusController::ModbusController() {}
ModbusController::~ModbusController()
{
    terminate();
}


SystemResult ModbusController::SetInterfaceParameters(ModbusStrategy* modbusStrategyPtr, const ModbusConnectionParameters &cConnectionParameters)
{
    SystemResult retVal = SystemResult::SYSTEM_ERROR;

    retVal = modbusStrategyPtr->SetConnectionParameters(cConnectionParameters);

    if ( SystemResult::SYSTEM_ERROR == retVal )
    {
        /* If the object is TCP Client type log only IP address and port. */
        /* If any new modbus connection occurs add the corresponding logging there */
        if( dynamic_cast<ModbusTCPClient*>(modbusStrategyPtr) )
        {
            logger->LogCritical( TAG,
                                 "Unable to initialize " + modbusStrategyPtr->GetDeviceName() +
                                 " ID: " + QString::number(modbusStrategyPtr->GetDeviceID()) +
                                 " IP: " + cConnectionParameters.GetIpAddress() +
                                 " Port: " + QString::number(cConnectionParameters.GetPort()) );
        }
        else /* If unknown log all of them */
        {
            logger->LogCritical( TAG,
                                 "Unable to initialize " + modbusStrategyPtr->GetDeviceName() +
                                 " ID: " + QString::number(modbusStrategyPtr->GetDeviceID()) +
                                 " Parameters: SerialPort: " + cConnectionParameters.GetSerialPort() +
                                 " BaudRate: " + QString::number(cConnectionParameters.GetBaudRate()) +
                                 " DataBits: " + QString::number(cConnectionParameters.GetDataBits()) +
                                 " Parity: " + QString::number(cConnectionParameters.GetParity()) +
                                 " StopBits: " + QString::number(cConnectionParameters.GetStopBits()) +
                                 " IpAddress: " + cConnectionParameters.GetIpAddress() +
                                 " Port: " + QString::number(cConnectionParameters.GetPort()) );
        }
    }

    return retVal;
}

void ModbusController::InitializeInterfaces()
{
    for ( auto* interface : _modbusInterfacesList)
    {
        if ( !interface->IsConnected() )
            (void)interface->Connect();
    }
}


SystemResult ModbusController::AddInterface(ModbusStrategy* modbusStrategyPtr)
{
    SystemResult retVal = SystemResult::SYSTEM_OK;

    if(!modbusStrategyPtr)
        retVal = SystemResult::SYSTEM_ERROR;

    if(SystemResult::SYSTEM_OK != retVal)
        logger->LogCritical(TAG, NULL_PTR_MESSAGE);

    if(SystemResult::SYSTEM_OK == retVal)
        _modbusInterfacesList.append(modbusStrategyPtr);

    if(SystemResult::SYSTEM_OK != retVal && modbusStrategyPtr)
        logger->LogCritical(TAG, "Adding interface " + modbusStrategyPtr->GetDeviceName() + " failed!");

    return retVal;
}

SystemResult ModbusController::RemoveInterface(ModbusStrategy* modbusStrategyPtr)
{
    SystemResult retVal = SystemResult::SYSTEM_OK;

    if(!modbusStrategyPtr)
        retVal = SystemResult::SYSTEM_ERROR;

    if(SystemResult::SYSTEM_OK != retVal)
        logger->LogCritical(TAG, NULL_PTR_MESSAGE);

    if(SystemResult::SYSTEM_OK == retVal)
        _modbusInterfacesList.removeAt(modbusStrategyPtr->GetDeviceID());

    if(SystemResult::SYSTEM_OK != retVal && modbusStrategyPtr)
        logger->LogCritical(TAG, "Removing interface " + modbusStrategyPtr->GetDeviceName() + " failed!");

    return retVal;
}

void ModbusController::start(Priority priority)
{

}

void ModbusController::terminate()
{
    for (auto & interface : _modbusInterfacesList)
    {
        if ( interface->IsConnected() )
             (void) interface->Disconnect();
    }
}



QModbusReply *ModbusController::readRegister(ModbusStrategy* modbusStrategyPtr, QModbusDataUnit::RegisterType cDataUnit, int startingAddress, quint16 numberOfRegisters)
{
    QModbusReply * result = nullptr;

    QModbusDataUnit query(cDataUnit, startingAddress, numberOfRegisters);

    result = modbusStrategyPtr->ReadData(query);

    return result;
}


#warning TODO INPLEMENT!!!
QModbusReply *ModbusController::writeRegister(ModbusStrategy* modbusStrategyPtr, QModbusDataUnit::RegisterType cDataUnit, int startingAddress, quint16 numberOfRegisters)
{
    return nullptr;
}
