#ifndef APP_H
#define APP_H

#include <QCoreApplication>
#include <QApplication>
#include <memory>
#include <map>

#include <tagsystem/taglist.h>
#include <tagsystem/tag.h>

#include "websocketserver.h"
#include "mainwindow.h"

#include "pluginmanager.h"
#include <QNetworkAccessManager>


class QUdpSocket;
class QTimer;
class LogValueData;

#ifdef NO_GUI
class App : public QCoreApplication
#else
class App : public QApplication
#endif
{
public:
    App(int argc, char *argv[]);
    ~App();

private slots:
    void onSystemTimeTimer();
    void onLogEntry(QString message);

private:
    void loadPlugins();
    QNetworkAccessManager networkAcessManager_;
    WebSocketServer *mWebSocketServer;
#ifndef NO_GUI
    MainWindow *mMainWindow;
#endif

    LogValueData *logValueData_;
    Tag *mSystemTimeTag = nullptr;
    QTimer *mSystemTimeTimer = nullptr;

    PluginManager pluginManager_;
};

#endif // APP_H
