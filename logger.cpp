#include "logger.h"

Logger::Logger()
{

}


Logger& Logger::sGetInstance()
{
    static Logger sLogger;
    return sLogger;
}


void Logger::log(QString aMsg)
{
    emit logEntry(aMsg);
    //TODO: Log to disk.
}
