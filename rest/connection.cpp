#include "connection.h"

#include <QDebug>

Connection::Connection(QTcpSocket *socket) :
    socket_(socket)
{
    qDebug() << __FUNCTION__;
    connect(socket, &QTcpSocket::readyRead, this, &Connection::onReadyRead);
}

Connection::~Connection()
{
    socket_->deleteLater();
}

HttpRequest *Connection::httpRequest() const
{
    return request_;
}

void Connection::onReadyRead()
{
    request_ = new HttpRequest(socket_->readAll());
    qDebug() << request_->command();
    emit requestReady();
}
