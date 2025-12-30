#include "logvalueapi.h"

#include <QHttpServer>
#include <QHttpServerRequest>
#include <QJsonArray>
#include <QJsonObject>

#include <tagsystem/util/json.h>

#include "logvaluedata.h"

LogValueApi::LogValueApi(QHttpServer &httpServer, LogValueData &logValueData) :
    httpServer_(httpServer),
    logValueData_(logValueData)
{
    httpServer_.route("/api/logvalue/get", QHttpServerRequest::Method::Get,
        [this](const QHttpServerRequest &){ return get();});

    httpServer_.route("/api/logvalue/delete", QHttpServerRequest::Method::Post,
        [this](const QHttpServerRequest &request){return deleteValue(request);});

    httpServer_.route("/api/logvalue/add", QHttpServerRequest::Method::Post,
        [this](const QHttpServerRequest &request){return addValue(request);});
}

QHttpServerResponse LogValueApi::get()
{
    auto list = logValueData_.toJsonArray();
    return QHttpServerResponse(list);
}

// tagsocket: subsystem.name
QHttpServerResponse LogValueApi::deleteValue(const QHttpServerRequest &request)
{
    auto json = util::json::byteArrayToJsonObject(request.body());
    if(!json.has_value())
        return QHttpServerResponse(QHttpServerResponder::StatusCode::BadRequest);

    const QJsonObject obj = json.value();
    const auto tagsocket = obj.value("tagsocket").toString();
    logValueData_.removeLogValueByTagSocketName(tagsocket);

    QJsonObject respons;
    respons.insert("tagsocket", tagsocket);
    respons.insert("status", "deleted");
    QJsonDocument document(respons);

    return QHttpServerResponse(document.toJson());
}

// tagsocketsubsystem: tagsocket subsystem
// tagsocketname: tagsocket name
// tagsubsystem: tag subsystem
// tagname: tag name
QHttpServerResponse LogValueApi::addValue(const QHttpServerRequest &request)
{
    auto json = util::json::byteArrayToJsonObject(request.body());
    if(!json.has_value())
        return QHttpServerResponse(QHttpServerResponder::StatusCode::BadRequest);

    const QJsonObject obj = json.value();

    const auto tagsocketSubsystem = obj.value("tagsocketsubsystem").toString();
    const auto tagsocketName = obj.value("tagsocketname").toString();
    const auto tagSubsystem = obj.value("tagsubsystem").toString();
    const auto tagName = obj.value("tagname").toString();

    // validate data
    if(tagsocketSubsystem.isEmpty() || tagsocketName.isEmpty()
        || tagSubsystem.isEmpty() || tagName.isEmpty())
    {
        return QHttpServerResponse(QHttpServerResponder::StatusCode::BadRequest);
    }

    logValueData_.addLogValue(tagsocketSubsystem, tagsocketName, tagSubsystem, tagName);
    qDebug() << "number of logvalues " << logValueData_.numberOfLogVAlues();

    QJsonDocument document(obj);
    return QHttpServerResponse(document.toJson());
}
