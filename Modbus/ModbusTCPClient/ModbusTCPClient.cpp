#include <ModbusTCPClient.hpp>
#include <QVariant>
#include <Logger.hpp>

static auto* logger = &Singleton<Logger>::GetInstance();

ModbusTCPClient::ModbusTCPClient() : _modbusTCPClient(nullptr)
{
    this->SetDeviceName(MODBUS_TCP_DEVICE_NAME);
    _modbusTCPClient = std::make_unique<QModbusTcpClient>();

    logger->LogInfo( CLASS_TAG, "Created object with ID: " + QString::number(GetDeviceID()) );
}

ModbusTCPClient::~ModbusTCPClient()
{
    logger->LogInfo( CLASS_TAG, "Destroyed object with ID: " + QString::number(GetDeviceID()) );

    if (_modbusTCPClient)
        _modbusTCPClient->disconnectDevice();
}

SystemResult ModbusTCPClient::Connect()
{
    SystemResult retVal = SystemResult::SYSTEM_OK;

    if ( !_modbusTCPClient )
    {
        retVal = SystemResult::SYSTEM_ERROR;
        logger->LogCritical( CLASS_TAG, "Trying to connect with interface nullptr!");
    }

    if ( SystemResult::SYSTEM_ERROR != retVal )
    {
        ( void )_modbusTCPClient->setConnectionParameter(QModbusDevice::NetworkAddressParameter, QVariant::fromValue(_connectionParameters.GetIpAddress()));
        ( void )_modbusTCPClient->setConnectionParameter(QModbusDevice::NetworkPortParameter, QVariant::fromValue(_connectionParameters.GetPort()));
    }

    if ( SystemResult::SYSTEM_ERROR != retVal )
    {
        if(!_modbusTCPClient->connectDevice())
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

    if ( nullptr != _modbusTCPClient )
    {
        _modbusTCPClient->disconnectDevice();

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
    return SystemResult::SYSTEM_ERROR;
}

SystemResult ModbusTCPClient::WriteData(const QModbusDataUnit &cData)
{
    return SystemResult::SYSTEM_ERROR;
}
