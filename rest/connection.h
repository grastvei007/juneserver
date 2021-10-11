#ifndef CONNECTION_H
#define CONNECTION_H

#include <QTcpSocket>

#include "httprequest.h"

class Connection : public QObject
{
    Q_OBJECT
public:
    Connection(QTcpSocket *socket);
    ~Connection();

    HttpRequest* httpRequest() const;

signals:
    void requestReady();

private slots:
    void onReadyRead();

private:
    QTcpSocket *socket_;
    HttpRequest *request_ = nullptr;
};

#endif // CONNECTION_H
