#pragma once

#include <Singleton.hpp>
#include <QString>

class Logger : public Singleton<Logger>
{
    friend class Singleton<Logger>;
public:
    void LogDebug(const QString& cTag, const QString& cMessage);
    void LogInfo(const QString& cTag, const QString& cMessage);
    void LogWarning(const QString& cTag, const QString& cMessage);
    void LogCritical(const QString& cTag, const QString& cMessage);
    void LogFatal(const QString& cTag, const QString& cMessage);

private:
    Logger();
    ~Logger();

    QString _currentTime;
    static constexpr const char* TIME_STAMP_FORMAT {"yyyy-MM-dd HH:mm:ss.zzz"};

    static constexpr const char* DEBUG_TAG {"[Debug]"};
    static constexpr const char* INFO_TAG {"[INFO]"};
    static constexpr const char* WARNING_TAG {"[WARNING]"};
    static constexpr const char* CRITICAL_TAG {"[CRITICAL]"};
    static constexpr const char* FATAL_TAG {"[FATAL]"};
};

