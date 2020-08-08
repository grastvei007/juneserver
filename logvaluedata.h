#ifndef LOGVALUEDATA_H
#define LOGVALUEDATA_H

#include <QObject>
#include <vector>
#include <memory>

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
private:
    QString mTableName;
    QString mDataName;
    QString mTagSubSystem;
    QString mTagName;
};

#endif // LOGVALUEDATA_H
