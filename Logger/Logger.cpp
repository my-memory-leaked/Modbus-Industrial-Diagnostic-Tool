#include <Logger.hpp>
#include <QDebug>
#include <QDateTime>

Logger::Logger() {}
Logger::~Logger() {}

void Logger::LogDebug(const QString& cTag, const QString& cMessage)
{
    _currentTime = QDateTime::currentDateTime().toString(TIME_STAMP_FORMAT);
    qDebug() << _currentTime << DEBUG_TAG << cTag << cMessage;
}

void Logger::LogInfo(const QString& cTag, const QString& cMessage)
{
    _currentTime = QDateTime::currentDateTime().toString(TIME_STAMP_FORMAT);
    qInfo() << _currentTime << INFO_TAG << cTag << cMessage;
}

void Logger::LogWarning(const QString& cTag, const QString& cMessage)
{
    _currentTime = QDateTime::currentDateTime().toString(TIME_STAMP_FORMAT);
    qWarning() << _currentTime << WARNING_TAG << cTag << cMessage;
}

void Logger::LogCritical(const QString& cTag, const QString& cMessage)
{
    _currentTime = QDateTime::currentDateTime().toString(TIME_STAMP_FORMAT);

    qCritical() << _currentTime << CRITICAL_TAG << cTag << cMessage;
}

void Logger::LogFatal(const QString& cTag, const QString& cMessage)
{
    _currentTime = QDateTime::currentDateTime().toString(TIME_STAMP_FORMAT);
    qFatal() << _currentTime << FATAL_TAG << cTag << cMessage;
}
