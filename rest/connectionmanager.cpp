#include "connectionmanager.h"

ConnectionManager::ConnectionManager()
{
    connect(this, &ConnectionManager::newConnection, this, &ConnectionManager::onNewConnection);
}

void ConnectionManager::handleRequest(Connection *connection)
{
    std::unique_ptr<Connection> con(connection);
    connect(con.get(), &Connection::requestReady, this, &ConnectionManager::onNewConnection);

    connections_.push_back(std::move(con));
}

void ConnectionManager::addRequestListener(RequestListener *listener)
{
    requestListeners_.push_back(listener);
}

void ConnectionManager::onNewConnection()
{
    auto &connection = connections_.front();


    for(auto &listener : requestListeners_)
    {
        if(listener->isRequest(connection->httpRequest()->command()))
        {
            auto response = listener->executeRequest(connection->httpRequest()->command());
            //connection->sendRespons
            qDebug() << "Request found";
            break;
        }
    }



    connections_.erase(connections_.begin());
}
