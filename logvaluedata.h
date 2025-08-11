#ifndef LOGVALUEDATA_H
#define LOGVALUEDATA_H

#include <QObject>
#include <vector>
#include <memory>

#include <tagsystem/tagsocket.h>
#include <influxdb/influxdb.h>

class LogValue;
class QJsonObject;

class LogValueData : public QObject
{
    Q_OBJECT
public:
    explicit LogValueData(const QString &appName, InfluxDB &influxDb, QObject *parent = nullptr);

    void addLogValue(const QString &tableName, const QString &valueName, const QString &tagSubSystem, const QString &tagName);

    void saveLogValueList();
    void loadLogValueList();

    int numberOfLogVAlues() const;
    const LogValue* getLogValueByIndex(unsigned int index) const;
signals:
    void logValueAdded();
    void logValueRemoved();
    void logValueListSaved();
    void logValueListLoaded();

private:
    void deprecatedLoadLogValueList();
    InfluxDB &influxDb_;
    const QString &appName_;
    const QString configFile_{"juneserverlogtags.json"};

    std::vector<std::unique_ptr<LogValue>> logValues_;
};


class LogValue : public QObject
{
    Q_OBJECT
public:
    LogValue(InfluxDB &infuxDb, const QString &tableName, const QString &valueName, const QString &tagSubSystem, const QString &tagName);
    LogValue(InfluxDB &infuxDb, const QString &tableName, const QString &valueName, TagSocket::Type type, const QString &tagSubSystem, const QString &tagName);
    LogValue(const QJsonObject &json, InfluxDB &infuxDb);

    const QString& getTableName() const;
    const QString& getValueName() const;
    const QString& getTagSubsystem() const;
    const QString& getTagName() const;
    QString getTagSocketTypeStr() const;

    QJsonObject toJson() const;

private slots:
    void onTagSocketValueChanged(TagSocket *tagSocket);

private:
    InfluxDB &influxdb_;

    QString tableName_;
    QString valueName_;
    QString tagSubSystem_;
    QString tagName_;
    TagSocket* logValueTagSocket_;
};

#endif // LOGVALUEDATA_H
