#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QString>

class QWebSocket;


class Client : public QObject
{
    Q_OBJECT
public:
    Client(QWebSocket *aWebSocket, QObject *aParent=nullptr);
    ~Client();

    void close();

    QString getName() const;
    QString getIp() const;
signals:
    void disconnected(Client *aClient);


private slots:
    void onTextMessageRecieved(QString aMsg);
    void onBinaryMessageRecieved(QByteArray aMsg);
    void onDisconnected();

private:
    void onInfo(QString aMsg);

private:
    QWebSocket *mWebSocket;
    bool mInfoFlag; ///< wait for information about client.

    QString mClientName;
    QString mClientIp;
};

#endif // CLIENT_H
