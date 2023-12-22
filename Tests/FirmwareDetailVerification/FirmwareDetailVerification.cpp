#include <FirmwareDetailVerification.hpp>
#include <Logger.hpp>
#include <SystemResult.hpp>

#include <QCoreApplication>

static auto* logger = &Logger::GetInstance();

FirmwareDetailVerification::FirmwareDetailVerification()
{
    _mbController =  &ModbusController::GetInstance();
    if(!_mbController)
    {
        logger->LogCritical(TAG, "Modbus controller nullptr returned!");
    }

    _mbStrategy = _mbController->GetInterfaceByName("AUMA");
    if (!_mbStrategy)
    {
        logger->LogCritical(TAG, "Strategy nullptr returned!");
    }
    else
    {
        QString appDirPath = QCoreApplication::applicationDirPath() + _cJsonFilePath;

        logger->LogInfo(TAG, appDirPath);
        _mbStrategy->LoadRegistersFromJSON(appDirPath);
    }


}
FirmwareDetailVerification::~FirmwareDetailVerification() {}

void FirmwareDetailVerification::RunTest()
{
    logger->LogCritical(TAG, "TEST NOT IMPLEMENTED");

    SystemResult status = SystemResult::SYSTEM_OK;

    logger->LogInfo(TAG, "Startring test for: " + _mbStrategy->GetDeviceName());

    if (!_mbStrategy->IsConnected())
    {
        logger->LogCritical(TAG, "Can't run test for disconnected device!");
        return;
    }

    QString firmwareVersion = getFirmwareVersion();

}



QString FirmwareDetailVerification::getFirmwareVersion()
{
    return QString();
}
