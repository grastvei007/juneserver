#ifndef CLIENTLISTWIDGET_H
#define CLIENTLISTWIDGET_H

#include <QWidget>
#include <QListWidget>
#include <memory>

#include "client.h"

class ClientListWidget : public QWidget
{
    Q_OBJECT
public:
    ClientListWidget();
    ClientListWidget(QWidget *parent=nullptr);

    void addClient(Client *aClient);

private slots:
    void onClientDisconnected(Client *aClient);

private:
    std::unique_ptr<QListWidget> mListWidget;

};

#endif // CLIENTLISTWIDGET_H
