#include "tagsocketbindingapi.h"

#include <QHttpServer>
#include <QHttpServerResponse>
#include <QHttpServerRequest>
#include <QJsonArray>

#include <tagsystem/tagsocketlist.h>
#include <tagsystem/util/json.h>
#include <tagsystem/tagsocket.h>

TagSocketBindingApi::TagSocketBindingApi(QHttpServer &httpServer, TagSocketList &tagSocketList)
    : httpServer_(httpServer),
    tagSocketList_(tagSocketList)
{
    httpServer_.route("/api/tagsocket/get", QHttpServerRequest::Method::Get,
        [this](const QHttpServerRequest &request) {return get(); });

    httpServer_.route("/api/tagsocket/hookup", QHttpServerRequest::Method::Post,
        [this](const QHttpServerRequest& request){ return hookup(request); });
}

QHttpServerResponse TagSocketBindingApi::get()
{
    auto list = tagSocketList_.toJsonArray();
    return QHttpServerResponse(list);
}

// tagsocket:  subsystem.name
// tag:        subsystem.name
QHttpServerResponse TagSocketBindingApi::hookup(const QHttpServerRequest &request)
{
    const auto json = util::json::byteArrayToJsonObject(request.body());
    if(!json.has_value())
        return QHttpServerResponse(QHttpServerResponder::StatusCode::BadRequest);

    const QJsonObject obj = json.value();

    const QString tagsocketName = obj.value("tagsocket").toString();
    const QString tag = obj.value("tag").toString();

    const QString tagSubsystem = tag.split(".").first();
    const QString tagName = tag.split(".").last();

    if(tagSubsystem.isEmpty() || tagName.isEmpty())
        return QHttpServerResponse(QHttpServerResponder::StatusCode::BadRequest);

    TagSocket *tagsocket = tagSocketList_.getTagSocketByName(tagsocketName);
    if(!tagsocket)
        return QHttpServerResponse(QHttpServerResponder::StatusCode::BadRequest);

    tagsocket->hookupTag(tagSubsystem, tagName);
	tagSocketList_.saveBindingList();
    return QHttpServerResponse(tagsocket->toJson());
}
