#include "client.h"

#include <QWebSocket>
#include <QDebug>
#include <QByteArray>
#include <QXmlStreamAttributes>
#include <QXmlStreamReader>

#include <tagsystem/taglist.h>
#include <tagsystem/tag.h>



#include "clientinformation.h"

Client::Client(QWebSocket *aWebSocket, QObject *aParent) : QObject(aParent),
    mWebSocket(aWebSocket),
    mInfoFlag(false),
    mClientName("Unamed")
{
    connect(mWebSocket, &QWebSocket::textMessageReceived, this, &Client::onTextMessageRecieved);
    connect(mWebSocket, &QWebSocket::binaryMessageReceived, this, &Client::onBinaryMessageRecieved);
    connect(mWebSocket, &QWebSocket::disconnected, this, &Client::onDisconnected);

}

Client::~Client()
{

}


void Client::close()
{
    if(mWebSocket)
        mWebSocket->close();
}


void Client::sendBinaryMessage(const QByteArray &aMsg)
{
    mWebSocket->sendBinaryMessage(aMsg);
}

void Client::onTextMessageRecieved(QString aMsg)
{
    if(!mInfoFlag)
    {
        qDebug() << aMsg;
        onInfo(aMsg);
        return;
    }
}

/**
 * @brief Client::onBinaryMessageRecieved
 *
 * Recieve tag information, new tags created, tag values updated.
 *
 * @param aMsg
 */
void Client::onBinaryMessageRecieved(QByteArray aMsg)
{
    QXmlStreamReader stream(aMsg);
    while(!stream.atEnd() && !stream.hasError())
    {
        QXmlStreamReader::TokenType token = stream.readNext();
        if(token == QXmlStreamReader::StartDocument)
            continue;
        if(token == QXmlStreamReader::StartElement)
        {
            if(stream.name() == QString("create"))
            {
                while(!(stream.tokenType() == QXmlStreamReader::EndElement && stream.name() == QString("create")))
                {
                    if(stream.readNext() != QXmlStreamReader::StartElement)
                        continue;
                    if(stream.name() == QString("tag"))
                        createTags(stream);
                }
            }
            else if(stream.name() == QString("update"))
            {
                while(!(stream.tokenType() == QXmlStreamReader::EndElement && stream.name() == QString("update")))
                {
                    if(stream.readNext() != QXmlStreamReader::StartElement)
                        continue;
                    if(stream.name() == QString("tag"))
                    {
                        updateTags(stream);
                    }
                }
            }
        }
    }
}


void Client::onDisconnected()
{
    emit disconnected(this);
}

/**
 * @brief Client::onInfo
 * @param aMsg
 */
void Client::onInfo(QString aMsg)
{
    ClientInformation info(aMsg);
    if(info.hasError())
    {
        qDebug() << info.errorStr();
        return;
    }
    mClientName = info.getName();
    mClientIp = info.getIp();

    emit connectionEstablished(this);
}


QString Client::getName() const
{
    return mClientName;
}


QString Client::getIp() const
{
    return mClientIp;
}


void Client::createTags(QXmlStreamReader &aStream)
{
    QXmlStreamAttributes attribs = aStream.attributes();
    QString subsystem = attribs.value("subsystem").toString();
    QString name = attribs.value("name").toString();
    QString type = attribs.value("type").toString();
    qint64 timestamp = -1;
    if(attribs.hasAttribute("timestamp"))
        timestamp = attribs.value("timestamp").toLongLong();

    if(TagList::sGetInstance().findByTagName(QString("%1.%2").arg(subsystem).arg(name)))
        return;

    if(type == "Double")
    {
        auto value = attribs.value("value").toDouble();
        Tag *tag = TagList::sGetInstance().createTag(subsystem, name, Tag::eDouble, value);
        emit tagCreated(tag);
    }
    else if(type == "Int")
    {
        auto value = attribs.value("value").toInt();
        Tag *tag = TagList::sGetInstance().createTag(subsystem, name, Tag::eInt, value);
        emit tagCreated(tag);

    }
    else if(type == "Bool")
    {
        auto value = attribs.value("value").toInt() == 1 ? true : false;
        Tag *tag = TagList::sGetInstance().createTag(subsystem, name, Tag::eBool, value);
        emit tagCreated(tag);
    }
    else if(type == "String")
    {
        Tag *tag = TagList::sGetInstance().createTag(subsystem, name, Tag::eString);
        tag->setValue(attribs.value("value").toString(), timestamp);
        emit tagCreated(tag);
    }
    else if(type == "Time")
    {
        Tag *tag = TagList::sGetInstance().createTag(subsystem, name, Tag::eTime);
        tag->setValue(QDateTime::fromMSecsSinceEpoch(attribs.value("value").toLongLong()), timestamp);
        emit tagCreated(tag);
    }
}


void Client::updateTags(QXmlStreamReader &aStream)
{
    QXmlStreamAttributes attribs = aStream.attributes();
    QString subsystem = attribs.value("subsystem").toString();
    QString name = attribs.value("name").toString();
    QString fullName = QString("%1.%2").arg(subsystem).arg(name);
    qint64 timestamp = -1;
    if(attribs.hasAttribute("timestamp"))
         timestamp = attribs.value("timestamp").toLongLong();

    Tag *tag = TagList::sGetInstance().findByTagName(fullName);
    if(!tag)
        return;

    switch (tag->getType()) {
    case Tag::eDouble:
        tag->setValue(attribs.value("value").toDouble(), timestamp);
        emit tagUpdated(tag);
        break;
    case Tag::eInt:
        tag->setValue(attribs.value("value").toInt(), timestamp);
        emit tagUpdated(tag);
        break;
    case Tag::eBool:
        tag->setValue(attribs.value("value").toInt() == 1 ? true : false, timestamp);
        emit tagUpdated(tag);
        break;
    case Tag::eString:
        tag->setValue(attribs.value("value").toString(), timestamp);
        emit tagUpdated(tag);
        break;
    case Tag::eTime:
        tag->setValue(QDateTime::fromMSecsSinceEpoch(attribs.value("value").toLongLong()), timestamp);
        emit tagUpdated(tag);
        break;
    default:
        Q_UNREACHABLE();
    }

}
