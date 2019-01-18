#include "clientlistwidget.h"

#include <QVBoxLayout>
#include <QLabel>
#include <QDebug>


ClientListWidget::ClientListWidget(QWidget *parent) : QWidget(parent)
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    QLabel *label = new QLabel(this);
    label->setText("Active Connections");
    label->setVisible(true);
    layout->addWidget(label);

    mListWidget.reset(new QListWidget(this));
    mListWidget->setVisible(true);

    layout->addWidget(mListWidget.get());
    setLayout(layout);

}


void ClientListWidget::addClient(Client *aClient)
{
    mListWidget->addItem(aClient->getName());
    connect(aClient, &Client::disconnected, this, &ClientListWidget::onClientDisconnected);
}


void ClientListWidget::onClientDisconnected(Client *aClient)
{
    QList<QListWidgetItem *> items = mListWidget->findItems(aClient->getName(), Qt::MatchFixedString);
    for(int i=0; i<items.size(); ++i)
    {
        qDebug() << "remove: " << items.at(i)->text();
        mListWidget->removeItemWidget(items.at(i));
        delete items.at(i);
    }
}
