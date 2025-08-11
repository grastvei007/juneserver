#include "logvaluedata.h"

#include <QDir>
#include <QDebug>
#include <QFile>
#include <QXmlStreamWriter>
#include <QXmlStreamReader>
#include <QApplication>
#include <QJsonArray>
#include <QJsonObject>
#include <QTextStream>

#include <tagsystem/tagsocket.h>
#include <tagsystem/taglist.h>
#include <tagsystem/util/json.h>

#include <influxdb/influxdb.h>

#include <tagsystem/util/path.h>

LogValueData::LogValueData(const QString &appName, InfluxDB &influxDb, QObject *parent) : QObject(parent),
    appName_(appName),
    influxDb_(influxDb)
{
    loadLogValueList();
}


void LogValueData::addLogValue(const QString &tableName, const QString &valueName, const QString &tagSubSystem, const QString &tagName)
{
    logValues_.push_back(std::make_unique<LogValue>(influxDb_, tableName, valueName, tagSubSystem, tagName));
    saveLogValueList();
    emit logValueAdded();
}

void LogValueData::saveLogValueList()
{
    QString path = util::path::configDir(appName_);
    path.append(QDir::separator());
    path.append(configFile_);

    QFile file(path);
    if(!file.open(QIODevice::WriteOnly))
    {
        qDebug() << __FUNCTION__ << "Error opening file, " << path;
        return;
    }

    QJsonArray array;

    for(const auto& logValue : logValues_)
    {
        array.push_back(logValue->toJson());
    }

    QJsonObject obj;
    obj.insert("logvalues", array);
    QJsonDocument document(obj);

    QTextStream stream(&file);
    stream << document.toJson();
    file.close();

    emit logValueListSaved();
}

void LogValueData::loadLogValueList()
{
    QString path = util::path::configDir(appName_);
    path.append(QDir::separator());
    path.append(configFile_);

    QFile file(path);

    if (!file.exists())
    {
        // fall back to old style if file is not there.
        deprecatedLoadLogValueList();
        return;
    }

    if (!file.open(QIODevice::ReadOnly))
    {
        qDebug() << __FUNCTION__ << "Error opening file, " << path;
        return;
    }

    auto data = file.readAll();
    auto value = util::json::byteArrayToJsonObject(data);
    if (!value.has_value())
        return;
    QJsonObject object = value.value();
    const QJsonArray array = object.value("logvalues").toArray();
    for (const auto &logValue : array)
    {
        if(logValue.isObject())
            logValues_.push_back(std::make_unique<LogValue>(logValue.toObject(), influxDb_));
    }

    qDebug() << __FUNCTION__ << "N LogValues: " << logValues_.size();
}

void LogValueData::deprecatedLoadLogValueList()
{
    qDebug() << __FUNCTION__;
    QString path = QDir::homePath() + QDir::separator() + ".config" + QDir::separator() + "june";

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

                logValues_.push_back(std::make_unique<LogValue>(influxDb_, table, tagname, TagSocket::typeFromString(type), tagsubsystem, tagname));
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
    qDebug() << __FUNCTION__ << "N LogValues: " << logValues_.size();
    // if there has been a fallback to this, save the current loaded values
    // in nex json format, then this function can be removed later.

    saveLogValueList();
}


int LogValueData::numberOfLogVAlues() const
{
    if(logValues_.empty())
        return 0;
    return logValues_.size();
}

const LogValue *LogValueData::getLogValueByIndex(unsigned int aIndex) const
{
    if(aIndex > logValues_.size())
        return nullptr;

    return logValues_.at(aIndex).get();
}



LogValue::LogValue(InfluxDB &influxDb, const QString &tableName, const QString &valueName, const QString &tagSubSystem, const QString &tagName) :
    influxdb_(influxDb),
    tableName_(tableName),
    valueName_(valueName),
    tagSubSystem_(tagSubSystem),
    tagName_(tagName),
    logValueTagSocket_(nullptr)
{
    Tag *tag = TagList::sGetInstance().findByTagName(tagSubSystem, tagName);

    logValueTagSocket_ = TagSocket::createTagSocket(tableName, valueName, TagSocket::typeMatchingTag(tag));
    logValueTagSocket_->hookupTag(tag);

    connect(logValueTagSocket_, qOverload<TagSocket*>(&TagSocket::valueChanged), this, &LogValue::onTagSocketValueChanged);
}

LogValue::LogValue(InfluxDB &influxDb, const QString &tableName, const QString &valueName, TagSocket::Type type, const QString &tagSubSystem, const QString &tagName) :
    influxdb_(influxDb),
    tableName_(tableName),
    valueName_(valueName),
    tagSubSystem_(tagSubSystem),
    tagName_(tagName),
    logValueTagSocket_(nullptr)
{
    logValueTagSocket_ = TagSocket::createTagSocket(tableName, valueName, type);
    logValueTagSocket_->hookupTag(tagSubSystem, tagName);
    connect(logValueTagSocket_, qOverload<TagSocket*>(&TagSocket::valueChanged), this, &LogValue::onTagSocketValueChanged);
}

LogValue::LogValue(const QJsonObject &json, InfluxDB &infuxDb)
    : influxdb_(infuxDb)
{
    tableName_ = json.value("tagsocket").toString();
    valueName_ = json.value("name").toString();
    auto tagSocketType = TagSocket::typeFromString(json.value("type").toString());
    tagSubSystem_ = json.value("tagsubsystem").toString();
    tagName_ = json.value("tagname").toString();

    logValueTagSocket_ = TagSocket::createTagSocket(tableName_, valueName_, tagSocketType);
    logValueTagSocket_->hookupTag(tagSubSystem_, tagName_);
    connect(logValueTagSocket_, qOverload<TagSocket*>(&TagSocket::valueChanged), this, &LogValue::onTagSocketValueChanged);
}

const QString &LogValue::getTableName() const
{
    return tableName_;
}

const QString &LogValue::getValueName() const
{
    return valueName_;
}

const QString &LogValue::getTagSubsystem() const
{
    return tagSubSystem_;
}

const QString &LogValue::getTagName() const
{
    return tagName_;
}

QString LogValue::getTagSocketTypeStr() const
{
    if(logValueTagSocket_)
        return logValueTagSocket_->getTypeStr();
    return {};
}

QJsonObject LogValue::toJson() const
{
    QJsonObject object;
    object.insert("tagsocket", tableName_);
    object.insert("name", valueName_);
    object.insert("type", getTagSocketTypeStr());
    object.insert("tagsubsystem", tagSubSystem_);
    object.insert("tagname", tagName_);

    return object;
}

void LogValue::onTagSocketValueChanged(TagSocket *tagSocket)
{
    switch (tagSocket->getType()) {
        case TagSocket::eInt:
        {
            int val;
            tagSocket->readValue(val);
            QString str = QString("%1=%2").arg(tagSocket->getName(), QString::number(val));
            str.replace(QChar::Space, "");
            influxdb_.insert(tagSocket->getSubSystem(), str, tagSocket->getTag()->getMsSinceEpoc(), InfluxDB::eMiliSecond);
            break;
        }
        case TagSocket::eDouble:
        {
            double val;
            tagSocket->readValue(val);
            QString str = QString("%1=%2").arg(tagSocket->getName(), QString::number(val));
            str.replace(QChar::Space, "");
            influxdb_.insert(tagSocket->getSubSystem(), str, tagSocket->getTag()->getMsSinceEpoc(), InfluxDB::eMiliSecond);
            break;
        }
        default:
            break;

    }
}
