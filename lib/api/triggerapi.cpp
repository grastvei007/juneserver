#include "triggerapi.h"

#include <optional>

#include <QHttpServer>
#include <QHttpServerResponse>
#include <QJsonObject>
#include <QJsonArray>

#include <tagsystem/util/json.h>

#include "automation/automationmanager.h"


TriggerApi::TriggerApi(QHttpServer &httpServer, AutomationManager &automationManager) :
    httpServer_(httpServer),
    automationManager_(automationManager)
{
    httpServer_.route("/api/trigger/get", QHttpServerRequest::Method::Get,
            [this](const QHttpServerRequest &request){return triggerList(request);});

    httpServer_.route("/api/trigger/get",
                      QHttpServerRequest::Method::Post,
                      [this](const QHttpServerRequest &request) {
                          return getSelectedTriggers(request);
                      });

    httpServer_.route("/api/trigger/create", QHttpServerRequest::Method::Post,
            [this](const QHttpServerRequest &request){return createTrigger(request); });

    httpServer_.route("/api/trigger/remove", QHttpServerRequest::Method::Post,
            [this](const QHttpServerRequest &request){return removeTrigger(request); });

    httpServer_.route("/api/trigger/update", QHttpServerRequest::Method::Post,
            [this](const QHttpServerRequest &request){return updateTrigger(request);});

    httpServer_.route("/api/trigger/upload",
                      QHttpServerRequest::Method::Post,
                      [this](const QHttpServerRequest &request) {
                          return updloadTriggerFile(request);
                      });
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
    const auto byteArray = request.body();


    if (const auto json = util::json::byteArrayToJsonObject(byteArray); json.has_value())
    {
        const QJsonObject obj = json.value();
        if (automationManager_.createTrigger(obj))
        {
            return QHttpServerResponse(QHttpServerResponder::StatusCode::Accepted);
        }
    } else if (const auto json = util::json::byteArrayToJsonArray(byteArray); json.has_value())
    {
        const QJsonArray array = json.value();
        for (const auto &ref : array)
        {
            const QJsonObject &obj = ref.toObject();
            if (!automationManager_.createTrigger(obj))
            {
                return QHttpServerResponse(QHttpServerResponder::StatusCode::BadRequest);
            }
        }
        return QHttpServerResponse(QHttpServerResponder::StatusCode::Accepted);
    }


    return QHttpServerResponse(QHttpServerResponder::StatusCode::BadRequest);
}

QHttpServerResponse TriggerApi::triggerList(const QHttpServerRequest &request)
{
    Q_UNUSED(request);
    return QHttpServerResponse(automationManager_.toJsonArray());
}

// request is an jsonarray with jsonObject that contains a single triggerName each
QHttpServerResponse TriggerApi::getSelectedTriggers(const QHttpServerRequest &request)
{
    const auto json = util::json::byteArrayToJsonArray(request.body());
    if (!json.has_value())
        return QHttpServerResponse(QHttpServerResponder::StatusCode::BadRequest);

    const QJsonArray array = json.value();
    QJsonArray result;

    for (const auto &ref : array)
    {
        const QJsonObject &selectedTrigger = ref.toObject();
        const QString triggerName = selectedTrigger.value("triggername").toString();
        if (auto *trigger = automationManager_.findTriggerByName(triggerName); trigger != nullptr)
        {
            result.push_back(trigger->toJson());
        }
    }

    return QHttpServerResponse(result);
}

// triggername:
// values to update
// triggervalue:
// duration:
// enable:
QHttpServerResponse TriggerApi::updateTrigger(const QHttpServerRequest &request)
{
    const auto json = util::json::byteArrayToJsonArray(request.body());
    if(!json.has_value())
        return QHttpServerResponse(QHttpServerResponder::StatusCode::BadRequest);

    const QJsonArray array = json.value();
    for (const auto &ref : array)
    {
        const QJsonObject &obj = ref.toObject();
        if(obj.contains("triggername"))
        {
            automationManager_.updateTrigger(obj);
        }
    }
    return QHttpServerResponse(QHttpServerResponder::StatusCode::Accepted);
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

QHttpServerResponse TriggerApi::updloadTriggerFile(const QHttpServerRequest &request)
{
    const auto jsonArray = util::json::byteArrayToJsonArray(request.body());
    if (!jsonArray.has_value())
        return QHttpServerResponse(QHttpServerResponder::StatusCode::BadRequest);

    const QJsonArray array = jsonArray.value();

    for (const auto &trigger : array)
    {
        const QJsonObject &obj = trigger.toObject();
        automationManager_.createTrigger(obj);
    }

    return QHttpServerResponse(automationManager_.toJsonArray());
}
