#pragma once
#include <Singleton.hpp>
#include <ModbusStrategy.hpp>
#include <list>
#include <memory>

class JSONToDevicesConventer : public Singleton<JSONToDevicesConventer>
{
    friend class Singleton<JSONToDevicesConventer>;
public:
    std::list<std::shared_ptr<ModbusStrategy>> FromJson(const QByteArray &cJsonData);
    std::list<std::shared_ptr<ModbusStrategy>> FromJsonFile(const QString &cFilePath);
private:
    JSONToDevicesConventer();
    ~JSONToDevicesConventer();

    static constexpr const char TAG[] = "[JSONToDevicesConventer]";
};


