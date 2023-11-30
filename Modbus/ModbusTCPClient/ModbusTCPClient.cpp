#include <ModbusTCPClient.hpp>
#include <Logger.hpp>
#include <memory>
#include <QModbusTcpClient>
#include <QVariant>
#include <ModbusRegisterTypeMapper.hpp>
#include <ModbusParser.hpp>
#include <ModbusStateMapper.hpp>
#include <QThread>


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
            logger->LogInfo( CLASS_TAG, "Connecting to device ID: " + QString::number(GetDeviceID())
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

        logger->LogInfo( CLASS_TAG, "Disconnecing from device ID: " + QString::number(GetDeviceID())
                                       + " IP: " + GetConnectionParameters().GetIpAddress()
                                       + " Port: " +QString::number( GetConnectionParameters().GetPort()) );
        _isConnected = false;
    }
    else
    {
        retVal = SystemResult::SYSTEM_ERROR;
        logger->LogCritical( CLASS_TAG, "Trying to disconnect uninitialized device!" );
    }

    return retVal;
}

QModbusReply *ModbusTCPClient::ReadData(const QModbusDataUnit &cData)
{
    QModbusReply* retVal = nullptr;
    SystemResult result = SystemResult::SYSTEM_OK;

    if (! cData.isValid() )
    {
        result = SystemResult::SYSTEM_INVALID_ARGUMENT;
        logger->LogDebug( CLASS_TAG, "Invalid QModbusDataUnit!" );
    }


    if ( !IsConnected() )
    {
        result = SystemResult::SYSTEM_ERROR;
        logger->LogCritical( CLASS_TAG, "Trying to read data from disconnected device: "
                             + QString::number( GetDeviceID() ) );
    }

    if (SystemResult::SYSTEM_OK == result)
    {
        retVal = _modbusClient->sendReadRequest(cData, GetConnectionParameters().GetIpAddress().toInt());

        logger->LogDebug( CLASS_TAG,
                          "Requested reading: RegisterType: " +
                          ModbusRegisterTypeMapper::RegisterTypeToString(cData.registerType()) +
                          " Address: " + QString::number(cData.startAddress()) +
                          " IP Address: " + GetConnectionParameters().GetIpAddress());
    }

    return retVal;
}

QModbusReply *ModbusTCPClient::WriteData(const QModbusDataUnit &cData)
{
    QModbusReply* retVal = nullptr;
    SystemResult result = SystemResult::SYSTEM_ERROR;

    /* Check the validity of the provided data unit */
    if (!cData.isValid())
    {
        logger->LogDebug(CLASS_TAG, "Invalid QModbusDataUnit provided for writing!");
        result = SystemResult::SYSTEM_INVALID_ARGUMENT;
    }

    /* Check if the client is connected */
    if (!IsConnected() && SystemResult::SYSTEM_OK == result)
    {
        logger->LogCritical(CLASS_TAG,
                            "Trying to write data to a disconnected device: " +
                                QString::number(GetDeviceID()));
        result = SystemResult::SYSTEM_ERROR;
    }

    if (SystemResult::SYSTEM_OK == result)
    {
        /* Send the write request */
        retVal = _modbusClient->sendWriteRequest(cData, GetConnectionParameters().GetIpAddress().toInt());

        /* Log the write request */
        logger->LogDebug(CLASS_TAG,
                         "Requested writing: RegisterType: " +
                             ModbusRegisterTypeMapper::RegisterTypeToString(cData.registerType()) +
                             " Address: " + QString::number(cData.startAddress()) +
                             " IP Address: " + GetConnectionParameters().GetIpAddress());
    }

    return retVal;
}



void ModbusTCPClient::onModbusConnectionStateChanged(QModbusDevice::State state)
{
    _state = state;
    ModbusStateMapper* mbMapper = &ModbusStateMapper::GetInstance();
    QString cStateString = mbMapper->StateToString(state);;

    switch (state)
    {
    case QModbusDevice::UnconnectedState:
        ( void )reconnect();
        break;
    case QModbusDevice::ConnectingState:
        emitModbusStateUpdated(state);
        break;
    case QModbusDevice::ConnectedState:
        emitModbusStateUpdated(state);
        break;
    case QModbusDevice::ClosingState:
        emitModbusStateUpdated(state);
        break;
    default:
        emitModbusStateUpdated(state);
        break;
    }

    logger->LogInfo(CLASS_TAG, "Modbus device ID: " + QString::number(GetDeviceID()) +
                                   " IP: " + GetConnectionParameters().GetIpAddress() +
                                   " Port: " + QString::number(GetConnectionParameters().GetPort()) +
                                   " State changed: " + cStateString);
}

void ModbusTCPClient::initializeModbusClient()
{
    this->SetDeviceName(MODBUS_TCP_DEVICE_NAME);
    _modbusClient = std::make_unique<QModbusTcpClient>();
    _type = ModbusStrategy::ModbusInterfaceType::TCP;
}

void ModbusTCPClient::connectSignalsAndSlots() const
{
    connect(_modbusClient.get(), &QModbusTcpClient::stateChanged, this, &ModbusTCPClient::onModbusConnectionStateChanged);
}

void ModbusTCPClient::reconnect()
{
    /* If the device is connected or in any other state, we first disconnect it */
    if (this->IsConnected() ||
        this->_modbusClient->state()!= QModbusDevice::UnconnectedState)
    {
        (void)this->Disconnect();
    }

    QThread::sleep(250);

    logger->LogInfo(CLASS_TAG, "Reconnecting Modbus device ID: " + QString::number(GetDeviceID()) +
                                   " IP: " + GetConnectionParameters().GetIpAddress() +
                                   " Port: " + QString::number(GetConnectionParameters().GetPort()));

    /* Now, attempt to reconnect */
    if (SystemResult::SYSTEM_OK != Connect())
    {
        logger->LogWarning(CLASS_TAG, QString("Failed to reconnect to interface: %1").arg(GetDeviceName()));
    }

    QThread::sleep(500);
}
