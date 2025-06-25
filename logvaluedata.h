#ifndef LOGVALUEDATA_H
#define LOGVALUEDATA_H

#include <QObject>
#include <vector>
#include <memory>

#include <tagsystem/tagsocket.h>
#include <influxdb/influxdb.h>

class LogValue;

class LogValueData : public QObject
{
    Q_OBJECT
public:
    explicit LogValueData(const QString &appName, InfluxDB &influxDb, QObject *parent = nullptr);
#ifdef __arm__
    ~LogValueData();
#endif

    void addLogValue(const QString &aTableName, const QString &aValueName, const QString &aTagSubSystem, const QString &TagName);

    void saveLogValueList();
    void loadLogValueList();

    int numberOfLogVAlues() const;
    const LogValue* getLogValueByIndex(unsigned int aIndex) const;
signals:
    void logValueAdded();
    void logValueRemoved();
    void logValueListSaved();
    void logValueListLoaded();

private:
    InfluxDB &influxDb_;
    const QString &appName_;
#ifdef __arm__
    std::vector<LogValue*> mLogValues;
#else
    std::vector<std::unique_ptr<LogValue>> mLogValues;
#endif
};


class LogValue : public QObject
{
    Q_OBJECT
public:
    LogValue(InfluxDB &infuxDb, const QString &aTableName, const QString &aValueName, const QString &aTagSubSystem, const QString &aTagName);
    LogValue(InfluxDB &infuxDb, const QString &aTableName, const QString &aValueName, TagSocket::Type aType, const QString &aTagSubSystem, const QString &aTagName);

    const QString& getTableName() const;
    const QString& getValueNAme() const;
    const QString& getTagSubsystem() const;
    const QString& getTagName() const;
    QString getTagSocketTypeStr() const;

private slots:
    void onTagSocketValueChanged(TagSocket *aTagSocket);

private:
    InfluxDB &influxdb_;

    QString mTableName;
    QString mValueName;
    QString mTagSubSystem;
    QString mTagName;
    TagSocket* mLogValueTagSocket;
};

#endif // LOGVALUEDATA_H
