#ifndef PLUGINAPI_H
#define PLUGINAPI_H

class PluginManager;
class QHttpServer;
class  QHttpServerResponse;
class QHttpServerRequest;

class PluginApi
{
public:
    PluginApi(QHttpServer& httpServer, PluginManager& pluginManager);

private:
    QHttpServerResponse loadPluginRequest(const QHttpServerRequest &request);
    QHttpServerResponse unloadPluginRequest(const QHttpServerRequest &request);
    QHttpServerResponse getRunningPlugins(const QHttpServerRequest &request);
    QHttpServer& httpServer_;
    PluginManager& pluginManager_;
};

#endif // PLUGINAPI_H
