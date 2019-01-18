#ifndef CLIENTLISTWIDGET_H
#define CLIENTLISTWIDGET_H

#include <QWidget>

#include "client.h"

class ClientListWidget
{
public:
    ClientListWidget();

public slots:
    void onNewConnection(Client *aClient);
};

#endif // CLIENTLISTWIDGET_H
