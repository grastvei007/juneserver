#include "logvaluedata.h"

LogValueData::LogValueData(QObject *parent) : QObject(parent)
{

}

void LogValueData::addLogValue(const QString &aTableName, const QString &aValueName, const QString &aTagSubSystem, const QString &TagName)
{
    mLogValues.push_back(std::make_unique<LogValue>(aTableName, aValueName, aTagSubSystem, TagName));
    emit logValueAdded();
}

void LogValueData::saveLogValueList()
{

}

void LogValueData::loadLogValueList()
{

}

int LogValueData::numberOfLogVAlues() const
{
    if(mLogValues.empty())
        return 0;
    return mLogValues.size();
}

const LogValue *LogValueData::getLogValueByIndex(unsigned int aIndex) const
{
    if(aIndex < 0 || aIndex > mLogValues.size())
        return nullptr;
    return mLogValues.at(aIndex).get();
}



LogValue::LogValue(const QString &aTableName, const QString &aValueName, const QString &aTagSubSystem, const QString &aTagName) :
    mTableName(aTableName),
    mDataName(aValueName),
    mTagSubSystem(aTagSubSystem),
    mTagName(aTagName)
{

}
