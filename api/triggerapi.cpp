#include "triggerapi.h"

#include <optional>

#include <QHttpServer>
#include <QHttpServerResponse>
#include <QJsonObject>

#include <tagsystem/util/json.h>

#include "automation/automationmanager.h"


TriggerApi::TriggerApi(QHttpServer &httpServer, AutomationManager &automationManager) :
    httpServer_(httpServer),
    automationManager_(automationManager)
{
    httpServer_.route("/api/trigger", QHttpServerRequest::Method::Get,
            [this](const QHttpServerRequest &request){return triggerList(request);});

    httpServer_.route("/api/trigger/create", QHttpServerRequest::Method::Post,
            [this](const QHttpServerRequest &request){return createTrigger(request); });

    httpServer_.route("/api/trigger/remove", QHttpServerRequest::Method::Post,
            [this](const QHttpServerRequest &request){return removeTrigger(request); });

    httpServer_.route("/api/trigger/update", QHttpServerRequest::Method::Post,
            [this](const QHttpServerRequest &request){return updateTrigger(request);});
}

// comman values
// -------------
// type:
// subsystem:
// name:
// triggername:
//---------------
// trigger above and below
// ---------------
// triggervalue:
// ----------------
// trigger on time
// ---------------
// triggervalue:
// duration:
QHttpServerResponse TriggerApi::createTrigger(const QHttpServerRequest &request)
{
    const auto json = util::json::byteArrayToJsonObject(request.body());
    if(!json.has_value())
        return QHttpServerResponse(QHttpServerResponder::StatusCode::BadRequest);

    const QJsonObject obj = json.value();
    if(automationManager_.createTrigger(obj))
    {
        return QHttpServerResponse(QHttpServerResponder::StatusCode::Accepted);
    }

    return QHttpServerResponse(QHttpServerResponder::StatusCode::BadRequest);
}

QHttpServerResponse TriggerApi::triggerList(const QHttpServerRequest &request)
{
    Q_UNUSED(request);
    return QHttpServerResponse(automationManager_.toJsonArray());
}

// triggername:
// values to update
// triggervalue:
// duration:
QHttpServerResponse TriggerApi::updateTrigger(const QHttpServerRequest &request)
{
    const auto json = util::json::byteArrayToJsonObject(request.body());
    if(!json.has_value())
        return QHttpServerResponse(QHttpServerResponder::StatusCode::BadRequest);

    const QJsonObject obj = json.value();
    if(obj.contains("triggername"))
    {
        automationManager_.updateTrigger(obj);
        return QHttpServerResponse(QHttpServerResponder::StatusCode::Accepted);
    }

    return QHttpServerResponse(QHttpServerResponder::StatusCode::BadRequest);
}

// triggername:
QHttpServerResponse TriggerApi::removeTrigger(const QHttpServerRequest &request)
{
    const auto json = util::json::byteArrayToJsonObject(request.body());
    if(!json.has_value())
        return QHttpServerResponse(QHttpServerResponder::StatusCode::BadRequest);

    const QJsonObject obj = json.value();
    if(obj.contains("triggername"))
    {
        auto triggername = obj.value("triggername").toString();
        automationManager_.removeTrigger(triggername);
        return QHttpServerResponse(QHttpServerResponder::StatusCode::Accepted);
    }
    return QHttpServerResponse(QHttpServerResponder::StatusCode::BadRequest);
}
