#include <ModbusTCPClient.hpp>
#include <Logger.hpp>
#include <memory>
#include <QModbusTcpClient>
#include <QVariant>


static auto* logger = &Singleton<Logger>::GetInstance();

ModbusTCPClient::ModbusTCPClient()
{
    this->SetDeviceName(MODBUS_TCP_DEVICE_NAME);
    _modbusClient = std::make_unique<QModbusTcpClient>();

    logger->LogInfo( CLASS_TAG, "Created object with ID: " + QString::number(GetDeviceID()) );
}

ModbusTCPClient::~ModbusTCPClient()
{
    logger->LogInfo( CLASS_TAG, "Destroyed object with ID: " + QString::number(GetDeviceID()) );

    if (_modbusClient)
        _modbusClient->disconnectDevice();
}

SystemResult ModbusTCPClient::Connect()
{
    SystemResult retVal = SystemResult::SYSTEM_OK;

    if ( !_modbusClient )
    {
        retVal = SystemResult::SYSTEM_ERROR;
        logger->LogCritical( CLASS_TAG, "Trying to connect with interface nullptr!");
    }

    if ( SystemResult::SYSTEM_ERROR != retVal )
    {
        ( void )_modbusClient->setConnectionParameter(QModbusDevice::NetworkAddressParameter, QVariant::fromValue(_connectionParameters.GetIpAddress()));
        ( void )_modbusClient->setConnectionParameter(QModbusDevice::NetworkPortParameter, QVariant::fromValue(_connectionParameters.GetPort()));
    }

    if ( SystemResult::SYSTEM_ERROR != retVal )
    {
        if(!_modbusClient->connectDevice())
            retVal = SystemResult::SYSTEM_ERROR;
        else
            logger->LogInfo( CLASS_TAG, "Connected to device ID: " + QString::number(GetDeviceID())
                             + " IP: " + GetConnectionParameters().GetIpAddress()
                             + " Port: " + GetConnectionParameters().GetPort() );
    }

    return retVal;
}

SystemResult ModbusTCPClient::Disconnect()
{
    SystemResult retVal = SystemResult::SYSTEM_OK;

    if ( nullptr != _modbusClient )
    {
        _modbusClient->disconnectDevice();

        logger->LogInfo( CLASS_TAG, "Disconnected device ID: " + QString::number(GetDeviceID())
                                       + " IP: " + GetConnectionParameters().GetIpAddress()
                                       + " Port: " + GetConnectionParameters().GetPort() );
    }
    else
    {
        retVal = SystemResult::SYSTEM_ERROR;
        logger->LogCritical( CLASS_TAG, "Trying to disconnect uninitialized device!" );
    }

    return retVal;
}

SystemResult ModbusTCPClient::ReadData(const QModbusDataUnit &cData)
{
    SystemResult retVal = SystemResult::SYSTEM_OK;
    if (! cData.isValid() )
        retVal = SystemResult::SYSTEM_INVALID_ARGUMENT;
    else
        logger->LogDebug( CLASS_TAG, "Invalid modbus data query!" );


    if (SystemResult::SYSTEM_OK == retVal)
    {
//        _modbusClient->sendReadRequest(cData);
    }


    return retVal;
}

SystemResult ModbusTCPClient::WriteData(const QModbusDataUnit &cData)
{
    return SystemResult::SYSTEM_ERROR;
}
