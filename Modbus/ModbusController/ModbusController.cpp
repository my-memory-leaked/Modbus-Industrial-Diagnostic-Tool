#include <ModbusController.hpp>
#include <Logger.hpp>
#include <ModbusTCPClient.hpp>
#include <ModbusStateMapper.hpp>
#include <utility>

static auto* logger = &Singleton<Logger>::GetInstance();

ModbusController::ModbusController()
{
}

ModbusController::~ModbusController()
{
    DisconnectAllInterfaces();
}

SystemResult ModbusController::AddInterface(std::unique_ptr<ModbusStrategy> modbusStrategyPtr)
{
    SystemResult status = SystemResult::SYSTEM_OK;

    if(!modbusStrategyPtr)
    {
        logger->LogCritical(TAG, NULL_PTR_MESSAGE);
        status = SystemResult::SYSTEM_ERROR;
    }

    if(SystemResult::SYSTEM_OK == status)
    {
        QString deviceName = modbusStrategyPtr->GetDeviceName();
        if(_modbusInterfacesMap.find(deviceName) != _modbusInterfacesMap.end())
        {
            logger->LogCritical(TAG, "Interface with device name " + deviceName + " already exists!");
            status = SystemResult::SYSTEM_ERROR;
        }
        else
        {
            _modbusInterfacesMap.emplace(deviceName, std::move(modbusStrategyPtr));
        }
    }

    return status;
}

SystemResult ModbusController::RemoveInterface(const QString& deviceName)
{
    SystemResult status = SystemResult::SYSTEM_OK;

    if(_modbusInterfacesMap.erase(deviceName) == 0) // `erase` returns the number of elements removed
    {
        logger->LogCritical(TAG, "Interface with device name " + deviceName + " does not exist!");
        status = SystemResult::SYSTEM_ERROR;
    }

    return status;
}

ModbusStrategy* ModbusController::GetInterfaceByName(const QString& deviceName)
{
    ModbusStrategy* interfacePtr = nullptr;

    auto it = _modbusInterfacesMap.find(deviceName);
    if(it != _modbusInterfacesMap.end())
    {
        interfacePtr = it->second.get();
    }
    else
    {
        logger->LogWarning(TAG, "Interface with device name " + deviceName + " not found!");
    }

    return interfacePtr;
}

void ModbusController::ConnectAllInterfaces()
{
    for (auto& entry : _modbusInterfacesMap)
    {
        ModbusStrategy* interface = entry.second.get();
        if (!interface->IsConnected())
        {
            if (SystemResult::SYSTEM_OK != interface->Connect())
            {
                logger->LogWarning(TAG, QString("Failed to initialize interface: %1").arg(interface->GetDeviceName()));
            }
        }
    }
}

void ModbusController::DisconnectAllInterfaces()
{
    for (auto& entry : _modbusInterfacesMap)
    {
        ModbusStrategy* interface = entry.second.get();
        if (interface->IsConnected())
        {
            if (SystemResult::SYSTEM_OK != interface->Disconnect())
            {
                logger->LogWarning(TAG, QString("Failed to deinitialize interface: %1").arg(interface->GetDeviceName()));
            }
        }
    }
}

void ModbusController::ConnectInterface(const QString& cDeviceName)
{
    ModbusStrategy* modbusStrategyPtr = GetInterfaceByName(cDeviceName);
    if (modbusStrategyPtr && !modbusStrategyPtr->IsConnected())
    {
        if (SystemResult::SYSTEM_OK != modbusStrategyPtr->Connect())
        {
            logger->LogWarning(TAG, QString("Failed to initialize interface: %1").arg(cDeviceName));
        }
    }
}

void ModbusController::DisconnectInterface(const QString& cDeviceName)
{
    ModbusStrategy* modbusStrategyPtr = GetInterfaceByName(cDeviceName);
    if (modbusStrategyPtr && modbusStrategyPtr->IsConnected())
    {
        if (SystemResult::SYSTEM_OK != modbusStrategyPtr->Disconnect())
        {
            logger->LogWarning(TAG, QString("Failed to deinitialize interface: %1").arg(cDeviceName));
        }
    }
}


void ModbusController::UpdateDeviceList(QListWidget* listWidget)
{
    ModbusStrategy* interface = nullptr;

    listWidget->clear();

    for (auto& entry : _modbusInterfacesMap)
    {
        interface = entry.second.get();

        QString deviceInfo = QString("Device Name: %1 | IP Address: %2 | Port: %3 | State: %4")
                                 .arg(interface->GetDeviceName())
                                 .arg(interface->GetConnectionParameters().GetIpAddress())
                                 .arg(interface->GetConnectionParameters().GetPort())
                                 .arg(ModbusStateMapper::GetInstance().StateToString(interface->GetState()));

        listWidget->addItem(deviceInfo); // Add the information string to the list widget
    }
}

QModbusReply* ModbusController::readRegister(ModbusStrategy* modbusStrategyPtr, QModbusDataUnit::RegisterType cDataUnit, int startingAddress, quint16 numberOfRegisters)
{
    QModbusReply* result = nullptr;

    QModbusDataUnit query(cDataUnit, startingAddress, numberOfRegisters);
    result = modbusStrategyPtr->ReadData(query);

    return result;
}

QModbusReply* ModbusController::writeRegister(ModbusStrategy* modbusStrategyPtr, QModbusDataUnit::RegisterType cDataUnit, int startingAddress, quint16 numberOfRegisters)
{
    // TODO: Implement as per your requirements
    return nullptr;
}
