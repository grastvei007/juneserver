#ifndef APP_H
#define APP_H

#include <QCoreApplication>
#include <QApplication>
#include <memory>

#include <tagsystem/taglist.h>

#include <tagsystem/tag.h>

#include "websocketserver.h"
#include "mainwindow.h"



class QUdpSocket;
class QTimer;
class LogValueData;

class App : public QApplication
{
public:
    App(int argc, char *argv[]);
    ~App();

private slots:
    void onSystemTimeTimer();

private:
    void loadPlugins();

    WebSocketServer *mWebSocketServer;
    MainWindow *mMainWindow;

    LogValueData *mLogValueData;
    Tag *mSystemTimeTag = nullptr;
    QTimer *mSystemTimeTimer = nullptr;
};

#endif // APP_H
