#include "logvaluedata.h"

#include <QDir>
#include <QDebug>
#include <QFile>
#include <QXmlStreamWriter>
#include <QXmlStreamReader>
#include <QApplication>

#include <tagsystem/tagsocket.h>
#include <tagsystem/taglist.h>

#include <influxdb/influxdb.h>

LogValueData::LogValueData(QNetworkAccessManager &networkAccessManager, QObject *parent) : QObject(parent),
    networkAccessManager_(networkAccessManager)
{
    loadLogValueList();
}


#ifdef __arm__
LogValueData::~LogValueData()
{
    for(int i=0; i<mLogValues.size(); ++i)
        delete mLogValues[i];

    mLogValues.clear();
}
#endif

void LogValueData::addLogValue(const QString &aTableName, const QString &aValueName, const QString &aTagSubSystem, const QString &TagName)
{
#ifdef __arm__
    mLogValues.push_back(new LogValue(networkAccessManager_, aTableName, aValueName, aTagSubSystem, TagName));
#else
    mLogValues.push_back(std::make_unique<LogValue>(networkAccessManager_, aTableName, aValueName, aTagSubSystem, TagName));
#endif
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
    {
        qDebug() << __FUNCTION__ << "Error opening file, " << path;
        return;
    }

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
    qDebug() << __FUNCTION__;
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
    if(!file.open(QIODevice::ReadOnly))
    {
        qDebug() << __FUNCTION__ << "Error opening file, " << path;
        return;
    }

    QXmlStreamReader stream(&file);

    while(!stream.atEnd() && !stream.hasError())
    {
        QXmlStreamReader::TokenType token = stream.readNext();
        if(token == QXmlStreamReader::StartDocument)
            continue;
        if(token == QXmlStreamReader::StartElement)
        {
            if(stream.name() == QString("logvalues"))
                continue;
            if(stream.name() == QString("logvalue"))
            {
                QString table = stream.attributes().value("tagsocket").toString();
                QString valuename = stream.attributes().value("name").toString();
                QString type = stream.attributes().value("type").toString();
                QString tagsubsystem = stream.attributes().value("tagsubsystem").toString();
                QString tagname = stream.attributes().value("tagname").toString();

#ifdef __arm__
                mLogValues.push_back(new LogValue(networkAccessManager_, table, tagname, TagSocket::typeFromString(type), tagsubsystem, tagname));
#else
                mLogValues.push_back(std::make_unique<LogValue>(networkAccessManager_, table, tagname, TagSocket::typeFromString(type), tagsubsystem, tagname));
#endif
            }
        }
    }
    file.close();

    if(stream.hasError())
    {
        qDebug() << __FUNCTION__ << stream.errorString();
    }
    else
    {
        emit logValueAdded();
    }
    qDebug() << __FUNCTION__ << "N LogValues: " << mLogValues.size();
}


int LogValueData::numberOfLogVAlues() const
{
    if(mLogValues.empty())
        return 0;
    return mLogValues.size();
}

const LogValue *LogValueData::getLogValueByIndex(unsigned int aIndex) const
{
    if(aIndex > mLogValues.size())
        return nullptr;
#ifdef __arm__
    return mLogValues.at(aIndex);
#else
    return mLogValues.at(aIndex).get();
#endif
}



LogValue::LogValue(QNetworkAccessManager &networkAccessManager, const QString &aTableName, const QString &aValueName, const QString &aTagSubSystem, const QString &aTagName) :
    networkAccessManager_(networkAccessManager),
    mTableName(aTableName),
    mValueName(aValueName),
    mTagSubSystem(aTagSubSystem),
    mTagName(aTagName),
    mLogValueTagSocket(nullptr)
{
    influxdb_.useDb("june");
    QString tagname = QString("%1.%2").arg(aTagSubSystem).arg(aTagName);
    Tag *tag = TagList::sGetInstance().findByTagName(tagname);

    mLogValueTagSocket = TagSocket::createTagSocket(aTableName, aValueName, TagSocket::typeMatchingTag(tag));
    mLogValueTagSocket->hookupTag(tag);

    connect(mLogValueTagSocket, qOverload<TagSocket*>(&TagSocket::valueChanged), this, &LogValue::onTagSocketValueChanged);
}

LogValue::LogValue(QNetworkAccessManager &networkAccessManager, const QString &aTableName, const QString &aValueName, TagSocket::Type aType, const QString &aTagSubSystem, const QString &aTagName) :
    networkAccessManager_(networkAccessManager),
    mTableName(aTableName),
    mValueName(aValueName),
    mTagSubSystem(aTagSubSystem),
    mTagName(aTagName),
    mLogValueTagSocket(nullptr)
{
    influxdb_.useDb("june");
    mLogValueTagSocket = TagSocket::createTagSocket(aTableName, aValueName, aType);
    mLogValueTagSocket->hookupTag(aTagSubSystem, aTagName);
    connect(mLogValueTagSocket, qOverload<TagSocket*>(&TagSocket::valueChanged), this, &LogValue::onTagSocketValueChanged);
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

void LogValue::onTagSocketValueChanged(TagSocket *aTagSocket)
{
    switch (aTagSocket->getType()) {
        case TagSocket::eInt:
        {
            int val;
            aTagSocket->readValue(val);
            QString str = QString("%1=%2").arg(aTagSocket->getName()).arg(QString::number(val));
            str.replace(QChar::Space, "");
            influxdb_.insert(aTagSocket->getSubSystem(), str, aTagSocket->getTag()->getMsSinceEpoc(), InfluxDB::eMiliSecond);
            break;
        }
        case TagSocket::eDouble:
        {
            double val;
            aTagSocket->readValue(val);
            QString str = QString("%1=%2").arg(aTagSocket->getName()).arg(QString::number(val));
            str.replace(QChar::Space, "");
            influxdb_.insert(aTagSocket->getSubSystem(), str, aTagSocket->getTag()->getMsSinceEpoc(), InfluxDB::eMiliSecond);
            break;
        }
        default:
            break;

    }
}
