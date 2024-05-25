#include "websocketserver.h"

#include <QDebug>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include <QXmlStreamAttributes>

#include <tagsystem/taglist.h>
#include <tagsystem/tag.h>

#include  "logger.h"

WebSocketServer::WebSocketServer(qint16 port, QString aServerName, QObject *parent) : QObject(parent)
{
    mWebSocketServer.reset(new QWebSocketServer(aServerName, QWebSocketServer::NonSecureMode, this));

    if (mWebSocketServer->listen(QHostAddress::Any, port))
    {
        qDebug() << aServerName << " is listening on port: " << port;
        connect(mWebSocketServer.get(), &QWebSocketServer::newConnection, this, &WebSocketServer::onNewConnection);
        connect(mWebSocketServer.get(), &QWebSocketServer::closed, this, &WebSocketServer::onServerClosed);
    }

    connect(&TagList::sGetInstance(), &TagList::tagValueChanged, this, &WebSocketServer::onTagValueChanged);

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
    if(!webSocket)
        return;

    Logger::sGetInstance().log(QString("New connection"));

    Client *client = new Client(webSocket);
    connect(client, &Client::connectionEstablished, this, &WebSocketServer::onConnectionEstablished);
}


void WebSocketServer::onServerClosed()
{
    for(auto &client : mClients)
    {
        client->close();
        client->deleteLater();
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
void WebSocketServer::processBinaryMessage(QByteArray /*aMsg*/)
{


    if(mTagsCreatedQueue.size() > 0)
        sendTagsCreatedToClients();
    if(mTagsUpdatedQueue.size() > 0)
        sendTagsUpdatedToClients();
}

/**
 * @brief WebSocketServer::sendTagsCreatedToClients
 *
 * Send all tags in created queue to all connected clients.
 */
void WebSocketServer::sendTagsCreatedToClients()
{

    QByteArray data;
    QXmlStreamWriter stream(&data);
    stream.setAutoFormatting(true);
    stream.writeStartDocument();
    stream.writeStartElement("create");

    for(auto &createdTag : mTagsCreatedQueue)
    {
        createdTag->writeToXml(stream);
    }
    mTagsCreatedQueue.clear();

    stream.writeEndElement();
    stream.writeEndDocument();

    for(auto &client : mClients)
    {
        client->sendBinaryMessage(data);
    }
}

/**
 * @brief WebSocketServer::sendTagsUpdatedToClients
 *
 * Send all tags in update queue to all connected clients.
 */
void WebSocketServer::sendTagsUpdatedToClients()
{
    QByteArray data;
    QXmlStreamWriter stream(&data);
    stream.setAutoFormatting(true);
    stream.writeStartDocument();
    stream.writeStartElement("update");

    for(auto &updatedTag : mTagsUpdatedQueue)
    {
        updatedTag->writeToXml(stream);
    }
    mTagsUpdatedQueue.clear();

    stream.writeEndElement();
    stream.writeEndDocument();

    for(auto &client : mClients)
    {
        client->sendBinaryMessage(data);
    }
}


void WebSocketServer::onTagValueChanged(Tag *aTag)
{
    mTagsUpdatedQueue.removeAll(aTag);
    mTagsUpdatedQueue.push_back(aTag);
    sendTagsUpdatedToClients();
}


void WebSocketServer::onConnectionEstablished(Client *aClient)
{
    mClients.push_back(aClient);
    emit newConnection(aClient);
    connect(aClient, &Client::disconnected, this, &WebSocketServer::onClientDisconnect);

    // send current initial tags list, ask client to create everything.
    QByteArray taglist;
    TagList::sGetInstance().toXml(taglist, true);
    aClient->sendBinaryMessage(taglist);

}

void WebSocketServer::onClientDisconnect(Client *client)
{
    auto clientName = client->getName();
    mClients.removeAll(client);
    client->deleteLater();
    Logger::sGetInstance().log(QString("Client: %1 disconnected").arg(clientName));
    Logger::sGetInstance().log(QString("Active connections (%1)").arg(mClients.size()));
}
