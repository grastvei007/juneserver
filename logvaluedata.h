#ifndef LOGVALUEDATA_H
#define LOGVALUEDATA_H

#include <QObject>
#include <vector>
#include <memory>

#include <tagsystem/tagsocket.h>

class LogValue;

class LogValueData : public QObject
{
    Q_OBJECT
public:
    explicit LogValueData(QObject *parent = nullptr);

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
    std::vector<std::unique_ptr<LogValue>> mLogValues;
};


class LogValue : public QObject
{
    Q_OBJECT
public:
    LogValue(const QString &aTableName, const QString &aValueName, const QString &aTagSubSystem, const QString &aTagName);
    LogValue(const QString &aTableName, const QString &aValueName, TagSocket::Type aType, const QString &aTagSubSystem, const QString &aTagName);

    const QString& getTableName() const;
    const QString& getValueNAme() const;
    const QString& getTagSubsystem() const;
    const QString& getTagName() const;
    QString getTagSocketTypeStr() const;
private:
    QString mTableName;
    QString mValueName;
    QString mTagSubSystem;
    QString mTagName;
    TagSocket* mLogValueTagSocket;
};

#endif // LOGVALUEDATA_H
