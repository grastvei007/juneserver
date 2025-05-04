#ifndef TRIGGERAPI_H
#define TRIGGERAPI_H

class QHttpServer;
class QHttpServerResponse;
class QHttpServerRequest;
class AutomationManager;


class TriggerApi
{
public:
    TriggerApi(QHttpServer &httpServer, AutomationManager &automationManager);

private:
    QHttpServerResponse createTrigger(const QHttpServerRequest &request);
    QHttpServerResponse triggerList(const QHttpServerRequest &request);
    QHttpServerResponse updateTrigger(const QHttpServerRequest &request);
    QHttpServerResponse removeTrigger(const QHttpServerRequest &request);

    QHttpServer &httpServer_;
    AutomationManager &automationManager_;
};

#endif // TRIGGERAPI_H
