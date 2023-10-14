#include <ModbusTCPClient.hpp>
#include <Logger.hpp>
#include <memory>
#include <QModbusTcpClient>
#include <QVariant>
#include <ModbusRegisterTypeMapper.hpp>


static auto* logger = &Singleton<Logger>::GetInstance();

ModbusTCPClient::ModbusTCPClient()
{
    initializeModbusClient();
    connectSignalsAndSlots();

    logger->LogInfo( CLASS_TAG, "Created object with ID: " + QString::number(GetDeviceID()) );
}

ModbusTCPClient::~ModbusTCPClient()
{
    if (_modbusClient)
        _modbusClient->disconnectDevice();

    logger->LogInfo( CLASS_TAG, "Destroyed object with ID: " + QString::number(GetDeviceID()) );
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
                             + " Port: " + QString::number(GetConnectionParameters().GetPort()) );
        _isConnected = true;
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
        _isConnected = false;
    }
    else
    {
        retVal = SystemResult::SYSTEM_ERROR;
        logger->LogCritical( CLASS_TAG, "Trying to disconnect uninitialized device!" );
    }

    return retVal;
}

#warning TODO GET THE REPLY!!!
SystemResult ModbusTCPClient::ReadData(const QModbusDataUnit &cData)
{
    SystemResult retVal = SystemResult::SYSTEM_OK;
    if (! cData.isValid() )
        retVal = SystemResult::SYSTEM_INVALID_ARGUMENT;
    else
        logger->LogDebug( CLASS_TAG, "Invalid QModbusDataUnit!" );


    if (SystemResult::SYSTEM_OK == retVal)
    {
        _modbusClient->sendReadRequest(cData, GetConnectionParameters().GetIpAddress().toInt());

        logger->LogDebug( CLASS_TAG,
                          "Requested reading: RegisterType: " +
                          ModbusRegisterTypeMapper::RegisterTypeToString(cData.registerType()) +
                          " Address: " + QString::number(cData.startAddress()) +
                          " IP Address: " + GetConnectionParameters().GetIpAddress());
    }

    return retVal;
}

#warning TODO IMPLEMENT!!!!
SystemResult ModbusTCPClient::WriteData(const QModbusDataUnit &cData)
{
    return SystemResult::SYSTEM_ERROR;
}


void ModbusTCPClient::onModbusConnectionStateChanged(QModbusDevice::State state)
{
    QString stateString;

    switch (state)
    {
    case QModbusDevice::UnconnectedState:
        stateString = "Disconnected";
        break;
    case QModbusDevice::ConnectingState:
        stateString = "Connecting";
        break;
    case QModbusDevice::ConnectedState:
        stateString = "Connected";
        break;
    case QModbusDevice::ClosingState:
        stateString = "Closing";
        break;
    default:
        stateString = "Unknown state";
        break;
    }

    logger->LogInfo(CLASS_TAG, "Modbus device ID: " + QString::number(GetDeviceID()) +
                                   " IP: " + GetConnectionParameters().GetIpAddress() +
                                   " Port: " + QString::number(GetConnectionParameters().GetPort()) +
                                   " State changed: " + stateString);
}

void ModbusTCPClient::initializeModbusClient()
{
    this->SetDeviceName(MODBUS_TCP_DEVICE_NAME);
    _modbusClient = std::make_unique<QModbusTcpClient>();
}

void ModbusTCPClient::connectSignalsAndSlots() const
{
    connect(_modbusClient.get(), &QModbusTcpClient::stateChanged, this, &ModbusTCPClient::onModbusConnectionStateChanged);

}
