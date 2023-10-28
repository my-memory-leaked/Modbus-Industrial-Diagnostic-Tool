#include <ModbusController.hpp>
#include <Logger.hpp>
#include <ModbusTCPClient.hpp>
#include <utility>

static auto* logger = &Singleton<Logger>::GetInstance();

ModbusController::ModbusController()
{
}

ModbusController::~ModbusController()
{
    terminateController();
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

void ModbusController::InitializeInterfaces()
{
    for (auto& entry : _modbusInterfacesMap)
    {
        ModbusStrategy* interface = entry.second.get();
        if (!interface->IsConnected())
            (void)interface->Connect();
    }
}

void ModbusController::startController()
{
    // Startup actions if required
    InitializeInterfaces();
}

void ModbusController::terminateController()
{
    for (auto& entry : _modbusInterfacesMap)
    {
        ModbusStrategy* interface = entry.second.get();
        if (interface->IsConnected())
            (void)interface->Disconnect();
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
