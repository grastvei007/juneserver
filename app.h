#ifndef APP_H
#define APP_H

#include <QCoreApplication>
#include <QApplication>
#include <memory>

#include <tagsystem/taglist.h>

#include "websocketserver.h"
#include "mainwindow.h"

class QUdpSocket;
class QTimer;

class App : public QApplication
{
public:
    App(int argc, char *argv[]);
    ~App();

private slots:
    void broadcast();


private:
    WebSocketServer *mWebSocketServer;
    QUdpSocket *mUdpSocet;
    QTimer *mBroadcastTimer;

    MainWindow *mMainWindow;
};

#endif // APP_H
