#pragma once

#include <Singleton.hpp>
#include <QString>
#include <QDir>

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

    QDir _logDir;
    QFile _logFile;

    QString _currentTime;
    static const QString TIME_STAMP_FORMAT;

    static const QString DEBUG_TAG;
    static const QString INFO_TAG;
    static const QString WARNING_TAG;
    static const QString CRITICAL_TAG;
    static const QString FATAL_TAG;

    static constexpr int MAX_LOG_FILE_ROTATION = 10;

    void openLogFile();
    void rotateLogs();
    void saveLogToFile(QString& log);
};

