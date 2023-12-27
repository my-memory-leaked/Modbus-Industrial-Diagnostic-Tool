#include <Logger.hpp>
#include <QDebug>
#include <QDateTime>

const QString Logger::TIME_STAMP_FORMAT = "yyyy-MM-dd HH:mm:ss.zzz";

const QString Logger::DEBUG_TAG = "[DEBUG]";
const QString Logger::INFO_TAG = "[INFO]";
const QString Logger::WARNING_TAG = "[WARNING]";
const QString Logger::CRITICAL_TAG = "[CRITICAL]";
const QString Logger::FATAL_TAG = "[FATAL]";


Logger::Logger()
{
    _logDir = QDir("Logs");

    if (!_logDir.exists())
        _logDir.mkpath(".");

    openLogFile();
    rotateLogs();
}
Logger::~Logger()
{
    if (_logFile.isOpen())
    {
        _logFile.close();
    }
}

void Logger::LogDebug(const QString& cTag, const QString& cMessage)
{
    _currentTime = QDateTime::currentDateTime().toString(TIME_STAMP_FORMAT);
    QString log = _currentTime + DEBUG_TAG  + cTag + cMessage;
    qDebug() << log;
    (void)saveLogToFile(log);
}

void Logger::LogInfo(const QString& cTag, const QString& cMessage)
{
    _currentTime = QDateTime::currentDateTime().toString(TIME_STAMP_FORMAT);
    QString log = _currentTime + INFO_TAG  + cTag + cMessage;
    qInfo() << log;
    (void)saveLogToFile(log);
}

void Logger::LogWarning(const QString& cTag, const QString& cMessage)
{
    _currentTime = QDateTime::currentDateTime().toString(TIME_STAMP_FORMAT);
    QString log = _currentTime + WARNING_TAG  + cTag + cMessage;
    qWarning() << log;
    (void)saveLogToFile(log);
}

void Logger::LogCritical(const QString& cTag, const QString& cMessage)
{
    _currentTime = QDateTime::currentDateTime().toString(TIME_STAMP_FORMAT);
    QString log = _currentTime + CRITICAL_TAG  + cTag + cMessage;
    qCritical() << log;
    (void)saveLogToFile(log);
}

void Logger::LogFatal(const QString& cTag, const QString& cMessage)
{
    _currentTime = QDateTime::currentDateTime().toString(TIME_STAMP_FORMAT);
    QString log = _currentTime + FATAL_TAG  + cTag + cMessage;
    qFatal() << log;
    (void)saveLogToFile(log);
}


void Logger::openLogFile()
{
    QString fileName = QDateTime::currentDateTime().toString("yyyyMMddHHmmss") + ".log";
    _logFile.setFileName(_logDir.filePath(fileName));
    _logFile.open(QIODevice::WriteOnly | QIODevice::Text);
}

void Logger::rotateLogs()
{
    QStringList logFiles = _logDir.entryList(QStringList() << "*.log", QDir::Files, QDir::Time);

    while (logFiles.size() > MAX_LOG_FILE_ROTATION)
    {
        _logDir.remove(logFiles.last());
        logFiles.removeLast();
    }
}

void Logger::saveLogToFile(QString& log)
{
    if (_logFile.isOpen())
    {
        QTextStream out(&_logFile);
        out << log << "\n";
    }
}
