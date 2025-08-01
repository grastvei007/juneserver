#ifndef APP_H
#define APP_H

#include <QCoreApplication>
#include <memory>

#include <tagsystem/taglist.h>
#include <tagsystem/tag.h>
#include <influxdb/influxdb.h>

#include "websocketserver.h"
#include "automation/automationmanager.h"

#include "pluginmanager.h"
#include <QNetworkAccessManager>
#include <QHttpServer>


class QUdpSocket;
class QTimer;
class LogValueData;
class PluginApi;
class TagApi;
class TriggerApi;
class TagSocketBindingApi;


class App : public QCoreApplication
{
public:
    App(int argc, char *argv[]);
    ~App();

private slots:
    void onSystemTimeTimer();
    void onLogEntry(QString message);

private:
    void loadPlugins();
    void setupHttpServer(quint16 port);
    QNetworkAccessManager networkAcessManager_;
    WebSocketServer *mWebSocketServer;
    QHttpServer httpServer_;
    std::unique_ptr<QTcpServer> tcpServer_;

    std::unique_ptr<PluginApi> pluginApi_;
    std::unique_ptr<TagApi> tagApi_;
    std::unique_ptr<TriggerApi> triggerApi_;
    std::unique_ptr<TagSocketBindingApi> tagSocketBindingApi_;

    LogValueData *logValueData_;
    Tag *mSystemTimeTag = nullptr;
    QTimer *mSystemTimeTimer = nullptr;

    PluginManager pluginManager_{httpServer_};
    InfluxDB influxdb_ = InfluxDB(networkAcessManager_);

    std::unique_ptr<AutomationManager> automationManager_;
};

#endif // APP_H
