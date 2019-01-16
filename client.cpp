#include "client.h"

#include <QWebSocket>

Client::Client(QWebSocket *aWebSocket) :
    mWebSocket(aWebSocket),
    mInfoFlag(false)
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

void Client::onTextMessageRecieved(QString aMsg)
{
    if(!mInfoFlag)
    {
        onInfo(aMsg);
        return;
    }
}


void Client::onBinaryMessageRecieved(QByteArray aMsg)
{

}


void Client::onDisconnected()
{

}

void Client::onInfo(QString aMsg)
{

}


QString Client::getName() const
{
    return mClientName;
}


QString Client::getIp() const
{
    return mClientIp;
}
