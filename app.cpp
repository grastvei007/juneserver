#include "app.h"

#include <QDebug>
#include <QCommandLineParser>
#include <QStringList>
#include <QtNetwork>
#include <QTimer>

#include <tagsystem/taglistview.h>

App::App(int argc, char *argv[]) : QApplication(argc, argv),
    mMainWindow(nullptr)
{
    mWebSocketServer = new WebSocketServer(5000, "JuneServer");
    setApplicationName("June Server");
    mUdpSocet = new QUdpSocket(this);
    QCommandLineParser parser;
    QCommandLineOption noGui(QStringList() << "g" << "no-gui", "Gui" );
    parser.addOption(noGui);

    parser.process(*this);

    if(!parser.isSet(noGui))
    {
        mMainWindow = new MainWindow;
        mMainWindow->setWindowTitle("June Server");
        mMainWindow->setCentralWidget(new TagListView());
        mMainWindow->show();

        connect(mWebSocketServer, &WebSocketServer::newConnection, mMainWindow, &MainWindow::onNewConnection);

    }

    mBroadcastTimer = new QTimer(this);
    connect(mBroadcastTimer, &QTimer::timeout, this, &App::broadcast);
    mBroadcastTimer->setInterval(1000*60);
    mBroadcastTimer->start();
    broadcast();
}

App::~App()
{
    if(mMainWindow)
        mMainWindow->deleteLater();
}


void App::broadcast()
{
    QString ip;
    const QHostAddress &localhost = QHostAddress(QHostAddress::LocalHost);
    for (const QHostAddress &address: QNetworkInterface::allAddresses()) {
        if (address.protocol() == QAbstractSocket::IPv4Protocol && address != localhost)
             ip = address.toString();
    }
    QByteArray msg("juneserveronline:");
    msg.append(ip);
    mUdpSocet->writeDatagram(msg, QHostAddress::Broadcast, 45454);
}
