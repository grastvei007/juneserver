#include "websocketserver.h"

#include <QDebug>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include <QXmlStreamAttributes>

#include <tagsystem/taglist.h>
#include <tagsystem/tag.h>

WebSocketServer::WebSocketServer(qint16 port, QString aServerName, QObject *parent) : QObject(parent)
{
    mWebSocketServer.reset(new QWebSocketServer(aServerName, QWebSocketServer::NonSecureMode, this));

    if (mWebSocketServer->listen(QHostAddress::Any, port))
    {
        qDebug() << aServerName << " is listening on port: " << port;
        connect(mWebSocketServer.get(), &QWebSocketServer::newConnection, this, &WebSocketServer::onNewConnection);
        connect(mWebSocketServer.get(), &QWebSocketServer::closed, this, &WebSocketServer::onServerClosed);
    }
}

/**
 * @brief WebSocketServer::onNewConnection
 *
 * A client is connected. The current taglist is sendt to it.
 *
 */
void WebSocketServer::onNewConnection()
{
    QWebSocket *webSocket = mWebSocketServer->nextPendingConnection();
    qDebug() << "New Connection ";
    connect(webSocket, &QWebSocket::textMessageReceived, this, &WebSocketServer::processTextMessage);
    connect(webSocket, &QWebSocket::binaryMessageReceived, this, &WebSocketServer::processBinaryMessage);
    connect(webSocket, &QWebSocket::disconnected, this, &WebSocketServer::socketDisconnected);

     mClients.push_back(webSocket);

     // send current initial tags list, ask client to create everything.
     QByteArray taglist;
     TagList::sGetInstance().toXml(taglist, true);
     webSocket->sendBinaryMessage(taglist);
}


void WebSocketServer::onServerClosed()
{
    for(int i=0; i<mClients.size(); ++i)
    {
        mClients[i]->close();
        mClients[i]->deleteLater();
    }
    mClients.clear();
}


void WebSocketServer::processTextMessage(QString aMsg)
{
    qDebug() << __FUNCTION__ << aMsg;
}

/**
 * @brief WebSocketServer::processBinaryMessage
 * @param aMsg
 *
 * Recieve a xml from a client that can contain new tags to be created
 * or tags with updated values.
 *
 */
void WebSocketServer::processBinaryMessage(QByteArray aMsg)
{
  //  qDebug() << __FUNCTION__ << QString(aMsg);
    QXmlStreamReader stream(aMsg);

    while(!stream.atEnd() && !stream.hasError())
    {
        QXmlStreamReader::TokenType token = stream.readNext();
        if(token == QXmlStreamReader::StartDocument)
            continue;
        if(token == QXmlStreamReader::StartElement)
        {
            if(stream.name() == "create")
            {
                while(!(stream.tokenType() == QXmlStreamReader::EndElement &&
                        stream.name() == "create"))
                {
                    if(stream.readNext() != QXmlStreamReader::StartElement)
                        continue;

                    if(stream.name() == "tag")
                    {
                        createTag(stream);
                    }
                }
            }
            else if(stream.name() == "update")
            {
                while(!(stream.tokenType() == QXmlStreamReader::EndElement &&
                        stream.name() == "update"))
                {
                    if(stream.readNext() != QXmlStreamReader::StartElement)
                        continue;

                    if(stream.name() == "tag")
                    {
                        updateTag(stream);
                    }
                }
            }
        }
    }

    if(stream.hasError())
    {
        qDebug() << __FUNCTION__ << "Error in xml file";
        qDebug() << __FUNCTION__ << stream.lineNumber();
        qDebug() << __FUNCTION__ << stream.errorString();
    }

    if(mTagsCreatedQueue.size() > 0)
        sendTagsCreatedToClients();
    if(mTagsUpdatedQueue.size() > 0)
        sendTagsUpdatedToClients();
}

/**
 * @brief WebSocketServer::socketDisconnected
 *
 * Client disconnected from server.
 */
void WebSocketServer::socketDisconnected()
{
    QWebSocket *client = qobject_cast<QWebSocket *>(sender());
    qDebug() << "socketDisconnected:" << client;
    if (client)
    {
        mClients.removeAll(client);
        client->deleteLater();
    }
}

/**
 * @brief WebSocketServer::createTag
 * @param aStream
 *
 * Create a new tag, and add the tag to create queue.
 */
void WebSocketServer::createTag(QXmlStreamReader &aStream)
{
    QXmlStreamAttributes attribs = aStream.attributes();
    QString subsystem = attribs.value("subsystem").toString();
    QString name = attribs.value("name").toString();
    QString type = attribs.value("type").toString();

    if(TagList::sGetInstance().findByTagName(QString("%1.%2").arg(subsystem).arg(name)))
        return;

    if(type == "Double")
    {
        Tag *tag = TagList::sGetInstance().createTag(subsystem, name, Tag::eDouble);
        tag->setValue(attribs.value("value").toDouble());
        mTagsCreatedQueue.push_back(tag);
        connect(tag, &Tag::valueChanged, this, &WebSocketServer::onTagValueChanged);
    }
    else if(type == "Int")
    {
        Tag *tag = TagList::sGetInstance().createTag(subsystem, name, Tag::eInt);
        tag->setValue(attribs.value("value").toInt());
        mTagsCreatedQueue.push_back(tag);
        connect(tag, &Tag::valueChanged, this, &WebSocketServer::onTagValueChanged);
    }
    else if(type == "Bool")
    {
        Tag *tag = TagList::sGetInstance().createTag(subsystem, name, Tag::eBool);
        tag->setValue(attribs.value("value").toInt() == 1 ? true : false);
        mTagsCreatedQueue.push_back(tag);
        connect(tag, &Tag::valueChanged, this, &WebSocketServer::onTagValueChanged);
    }

}

/**
 * @brief WebSocketServer::updateTag
 * @param aStream
 *
 * Update tag value in taglist and add the tag to the update queue.
 */
void WebSocketServer::updateTag(QXmlStreamReader &aStream)
{
    QXmlStreamAttributes attribs = aStream.attributes();
    QString subsystem = attribs.value("subsystem").toString();
    QString name = attribs.value("name").toString();

    QString fullname = QString("%1.%2").arg(subsystem).arg(name);
    Tag *tag = TagList::sGetInstance().findByTagName(fullname);
    if(tag)
    {
        switch (tag->getType()) {
        case Tag::eDouble :
            tag->setValue(attribs.value("value").toDouble());
            mTagsUpdatedQueue.push_back(tag);
            break;
        case Tag::eInt:
            tag->setValue(attribs.value("value").toInt());
            mTagsUpdatedQueue.push_back(tag);
            break;
        case Tag::eBool:
            tag->setValue(attribs.value("value").toInt() == 1 ? true : false);
            mTagsUpdatedQueue.push_back(tag);
            break;
        default:
            Q_UNREACHABLE();
            break;
        }
    }
}

/**
 * @brief WebSocketServer::sendTagsCreatedToClients
 *
 * Send all tags in created queue to all connected clients.
 */
void WebSocketServer::sendTagsCreatedToClients()
{

    qDebug() << "sending n: " << mTagsCreatedQueue.size() << " created tags";
    QByteArray data;
    QXmlStreamWriter stream(&data);
    stream.setAutoFormatting(true);
    stream.writeStartDocument();
    stream.writeStartElement("create");

    for(int i=0; i<mTagsCreatedQueue.size(); ++i)
    {
        mTagsCreatedQueue[i]->writeToXml(stream);
    }
    mTagsCreatedQueue.clear();

    stream.writeEndElement();
    stream.writeEndDocument();

    for(int i=0; i<mClients.size(); ++i)
    {
        mClients[i]->sendBinaryMessage(data);
    }
}

/**
 * @brief WebSocketServer::sendTagsUpdatedToClients
 *
 * Send all tags in update queue to all connected clients.
 */
void WebSocketServer::sendTagsUpdatedToClients()
{
    qDebug() << "sending n: " << mTagsUpdatedQueue.size() << " updated tags";
    QByteArray data;
    QXmlStreamWriter stream(&data);
    stream.setAutoFormatting(true);
    stream.writeStartDocument();
    stream.writeStartElement("update");

    for(int i=0; i<mTagsUpdatedQueue.size(); ++i)
    {
        mTagsUpdatedQueue[i]->writeToXml(stream);
    }
    mTagsUpdatedQueue.clear();

    stream.writeEndElement();
    stream.writeEndDocument();

    for(int i=0; i<mClients.size(); ++i)
    {
        mClients[i]->sendBinaryMessage(data);
    }
}


void WebSocketServer::onTagValueChanged(Tag *aTag)
{
    mTagsUpdatedQueue.removeAll(aTag);
    mTagsUpdatedQueue.push_back(aTag);
    sendTagsUpdatedToClients();
}
