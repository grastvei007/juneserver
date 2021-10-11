#include "restserver.h"

#include <QDebug>
#include <QTcpSocket>

#include "connection.h"

RestServer::RestServer()
{
    tcpServer_.reset(new QTcpServer);
    connect(tcpServer_.get(), &QTcpServer::newConnection, this, &RestServer::onNewConnection);

    connectionManager_.reset(new ConnectionManager);
}

void RestServer::listen(short port)
{
    port_ = port;
    if(!tcpServer_->listen(QHostAddress::Any, port_))
    {
        qDebug() << "Rest Server fail to listen on port: " << port_;
        return;
    }
    qDebug() << "Rest Server listen on port: " << port_;


}

void RestServer::addRequestListener(RequestListener *requestListener)
{
    connectionManager_->addRequestListener(requestListener);
}

void RestServer::onNewConnection()
{
    qDebug() << "Request";
    QTcpSocket *clientConnection = tcpServer_->nextPendingConnection();
    Connection *connection = new Connection(clientConnection);

    connectionManager_->handleRequest(connection);
}

