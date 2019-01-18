#ifndef WEBSOCKETSERVER_H
#define WEBSOCKETSERVER_H

#include <QWebSocketServer>
#include <QWebSocket>
#include <QVector>
#include <QString>
#include <QByteArray>
#include <QXmlStreamReader>

#include "client.h"

#include <memory>

class Tag;

/**
 * @brief The WebSocketServer class
 *
 * This class make sure the tags are updated and distributed between all
 * connected clients.
 *
 */
class WebSocketServer : public QObject
{
    Q_OBJECT
public:
    WebSocketServer(qint16 port, QString aServerName, QObject *parent=nullptr);

signals:
    void newConnection(Client*);

private slots:
    void onNewConnection();
    void onServerClosed();
    //
    void processTextMessage(QString aMsg);
    void processBinaryMessage(QByteArray aMsg);
    void socketDisconnected();

    void onTagValueChanged(Tag *aTag);

private:
    void sendTagsCreatedToClients();
    void sendTagsUpdatedToClients();
private:
    std::unique_ptr<QWebSocketServer> mWebSocketServer;

    //QVector<QWebSocket*> mClients;
    QVector<Client*> mClients;

    QVector<Tag*> mTagsCreatedQueue;
    QVector<Tag*> mTagsUpdatedQueue;
};

#endif // WEBSOCKETSERVER_H
