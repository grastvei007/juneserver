#include "client.h"

#include <QWebSocket>
#include <QDebug>
#include <QByteArray>
#include <QXmlStreamAttributes>
#include <QXmlStreamReader>
#include <QStringView>

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


void Client::createTags(QXmlStreamReader &stream)
{
    Tag *tag = TagList::sGetInstance().createTag(stream);
    if(tag)
        emit tagCreated(tag);
}


void Client::updateTags(QXmlStreamReader &stream)
{
    Tag *tag = TagList::sGetInstance().updateTag(stream);
    if(tag)
        emit tagUpdated(tag);
}
