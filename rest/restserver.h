#ifndef RESTSERVER_H
#define RESTSERVER_H

#include <QTcpServer>

#include <memory>

#include "connectionmanager.h"
#include "requestlistener.h"

class RestServer : public QObject
{
    Q_OBJECT
public:
    RestServer();

    void listen(short port);

    void addRequestListener(RequestListener *requestListener);
private slots:
    void onNewConnection();

private:
    std::unique_ptr<QTcpServer> tcpServer_;
    std::unique_ptr<ConnectionManager> connectionManager_;
    short port_ = 0;

};

#endif // RESTSERVER_H
