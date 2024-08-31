#include "websocketserver.h"

#include <QDebug>

#include <QJsonArray>
#include <QJsonDocument>

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
 * @brief WebSocketServer::socketDisconnected
 *
 * Client disconnected from server.
 */
void WebSocketServer::socketDisconnected()
{
  /*  QWebSocket *client = qobject_cast<QWebSocket *>(sender());
    qDebug() << "socketDisconnected:" << client;
    if (client)
    {
        mClients.removeAll(client);
        client->deleteLater();
    }*/
}

void WebSocketServer::onTagValueChanged(Tag *aTag)
{
    QJsonArray array;
    array.push_back(aTag->toJson());
    QJsonDocument document(array);
    auto binaryData = document.toJson();

    for(int i=0; i<mClients.size(); ++i)
    {
        mClients[i]->sendBinaryMessage(binaryData);
    }
    aTag->resetUpdateFlag();
}


void WebSocketServer::onConnectionEstablished(Client *aClient)
{
    mClients.push_back(aClient);
    emit newConnection(aClient);
    connect(aClient, &Client::disconnected, this, &WebSocketServer::onClientDisconnect);

    // send current initial tags list, ask client to create everything.
    auto tagList = TagList::sGetInstance().toJson();
    QJsonDocument document(tagList);
    aClient->sendBinaryMessage(document.toJson());
}

void WebSocketServer::onClientDisconnect(Client *client)
{
    auto clientName = client->getName();
    mClients.removeAll(client);
    client->deleteLater();
    Logger::sGetInstance().log(QString("Client: %1 disconnected").arg(clientName));
    Logger::sGetInstance().log(QString("Active connections (%1)").arg(mClients.size()));
}
