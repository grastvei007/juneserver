#include "client.h"

#include <QWebSocket>
#include <QDebug>
#include <QByteArray>
#include <QStringView>

#include <QJsonDocument>
#include <QJsonArray>

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
    auto document = QJsonDocument::fromJson(aMsg);
    auto array = document.array();

    for (const auto &jsonRef : array)
    {
        const auto &jsonTag = jsonRef.toObject();
        auto tag = TagList::sGetInstance().UpdateOrCreateTag(jsonTag);
        // sync an updated tag with all clients.
        tag->setUpdatedFlag();
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
