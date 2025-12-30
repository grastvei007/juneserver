#include "logger.h"
#include <QTime>

Logger::Logger()
{

}


Logger& Logger::sGetInstance()
{
    static Logger sLogger;
    return sLogger;
}


void Logger::log(QString message)
{
    auto messageWithTimeStamp = QString("[%1] ").arg(QTime::currentTime().toString());
    messageWithTimeStamp.append(message);
    messageWithTimeStamp.append("\n");

    emit logEntry(messageWithTimeStamp);
    //TODO: Log to disk.
}
