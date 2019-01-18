#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QString>
#include <QXmlStreamReader>

class QWebSocket;
class Tag;


class Client : public QObject
{
    Q_OBJECT
public:
    Client(QWebSocket *aWebSocket, QObject *aParent=nullptr);
    ~Client();

    void close();

    QString getName() const;
    QString getIp() const;

    void sendBinaryMessage(const QByteArray &aMsg);
signals:
    void disconnected(Client *aClient);
    void tagCreated(Tag*);
    void tagUpdated(Tag*);
    void connectionEstablished(Client*); ///< emitted after client information is recieved.

private slots:
    void onTextMessageRecieved(QString aMsg);
    void onBinaryMessageRecieved(QByteArray aMsg);
    void onDisconnected();

private:
    void onInfo(QString aMsg);
    void createTags(QXmlStreamReader &aStream);
    void updateTags(QXmlStreamReader &aStream);

private:
    QWebSocket *mWebSocket;
    bool mInfoFlag; ///< wait for information about client.

    QString mClientName;
    QString mClientIp;
};

#endif // CLIENT_H
