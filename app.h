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
#include "rest/restserver.h"


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
    void loadPlugin(QString name);

private:
    void setupRestResponses();
    void loadPlugins();

    WebSocketServer *mWebSocketServer;
    MainWindow *mMainWindow;

    LogValueData *mLogValueData;
    Tag *mSystemTimeTag = nullptr;
    QTimer *mSystemTimeTimer = nullptr;

    PluginManager pluginManager_;
    std::unique_ptr<RestServer> restServer_;
};

#endif // APP_H
