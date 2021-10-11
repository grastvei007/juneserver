#ifndef CONNECTIONMANAGER_H
#define CONNECTIONMANAGER_H

#include <QTcpSocket>
#include <vector>

#include <memory>
#include "connection.h"
#include "requestlistener.h"

class ConnectionManager : public QObject
{
    Q_OBJECT
public:
    ConnectionManager();

    void handleRequest(Connection *connection);
    void addRequestListener(RequestListener *listener);

signals:
    void newConnection();

private slots:
    void onNewConnection();

private:
    std::vector<std::unique_ptr<Connection>> connections_;
    std::vector<RequestListener*> requestListeners_;
};

#endif // CONNECTIONMANAGER_H
