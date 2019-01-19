#ifndef LOGGER_H
#define LOGGER_H

#include <QObject>

class Logger : public QObject
{
    Q_OBJECT
public:
    static Logger& sGetInstance();

    void log(QString aMsg);

signals:
    void logEntry(QString);

private:
    Logger();
};

#endif // LOGGER_H
