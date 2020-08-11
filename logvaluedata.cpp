#include "logvaluedata.h"

#include <QDir>
#include <QDebug>
#include <QXmlStreamWriter>

#include <tagsystem/tagsocket.h>
#include <tagsystem/taglist.h>

LogValueData::LogValueData(QObject *parent) : QObject(parent)
{

}

void LogValueData::addLogValue(const QString &aTableName, const QString &aValueName, const QString &aTagSubSystem, const QString &TagName)
{
    mLogValues.push_back(std::make_unique<LogValue>(aTableName, aValueName, aTagSubSystem, TagName));
    saveLogValueList();
    emit logValueAdded();
}

void LogValueData::saveLogValueList()
{
#ifdef __linux__
    QString path = QDir::homePath() + QDir::separator() + ".config" + QDir::separator() + "june";

#else
    QString path = qApp->applicationDirPath();
#endif

    QDir dir(path);
    if(!dir.exists())
        QDir().mkpath(path);
    path.append(QDir::separator());
    path.append("juneserverlogtags.xml");
    QFile file(path);
    if(!file.open(QIODevice::WriteOnly))
        qDebug() << __FUNCTION__ << "Error opening file, " << path;

    QXmlStreamWriter stream(&file);
    stream.setAutoFormatting(true);
    stream.writeStartDocument();
    stream.writeStartElement("logvalues");

    for(const auto& logValue : mLogValues)
    {
        stream.writeStartElement("logvalue");
        stream.writeAttribute("tagsocket", logValue->getTableName());
        stream.writeAttribute("name", logValue->getValueNAme());
        stream.writeAttribute("type", logValue->getTagSocketTypeStr());
        stream.writeAttribute("tagsubsystem", logValue->getTagSubsystem());
        stream.writeAttribute("tagname", logValue->getTagName());

        stream.writeEndElement();
    }

    stream.writeEndElement();
    stream.writeEndDocument();

    file.close();
    emit logValueListSaved();
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
    mValueName(aValueName),
    mTagSubSystem(aTagSubSystem),
    mTagName(aTagName),
    mLogValueTagSocket(nullptr)
{
    QString tagname = QString("%1.%2").arg(aTagSubSystem).arg(aTagName);
    Tag *tag = TagList::sGetInstance().findByTagName(tagname);

    TagSocket::Type tagSocketType;
    switch (tag->getType())
    {
    case Tag::eInt:
        tagSocketType = TagSocket::eInt;
        break;
    case Tag::eDouble:
        tagSocketType = TagSocket::eDouble;
        break;
    case Tag::eBool:
        tagSocketType = TagSocket::eBool;
        break;
    case Tag::eString:
        tagSocketType = TagSocket::eString;
        break;

    default:
        break;

    }

    mLogValueTagSocket = TagSocket::createTagSocket(aTableName, aValueName, tagSocketType);
}

const QString &LogValue::getTableName() const
{
    return mTableName;
}

const QString &LogValue::getValueNAme() const
{
    return mValueName;
}

const QString &LogValue::getTagSubsystem() const
{
    return mTagSubSystem;
}

const QString &LogValue::getTagName() const
{
    return mTagName;
}

QString LogValue::getTagSocketTypeStr() const
{
    if(mLogValueTagSocket)
        return mLogValueTagSocket->getTypeStr();
    return QString();
}
