#include "tagapi.h"

#include <QHttpServer>
#include <QHttpServerResponse>
#include <QJsonObject>
#include <QString>
#include <QUrlQuery>

#include <tagsystem/taglist.h>
#include <tagsystem/util/json.h>
#include <tagsystem/tag.h>

#include <optional>

TagApi::TagApi(QHttpServer &httpServer, TagList &tagList)
    : httpServer_(httpServer)
    , tagList_(tagList)
{
    httpServer_.route("/api/tag/create", QHttpServerRequest::Method::Post,
            [this](const QHttpServerRequest &request) {return createTag(request); });

    httpServer_.route("/api/tag/update", QHttpServerRequest::Method::Post,
            [this](const QHttpServerRequest &request) {return update(request); });

    httpServer_.route("/api/tag/get", QHttpServerRequest::Method::Post,
            [this](const QHttpServerRequest &request) {return get(request); });
}

// subsystem :
// name :
// type :
// value :
// description :
QHttpServerResponse TagApi::createTag(const QHttpServerRequest &request)
{
    const auto json = util::json::byteArrayToJsonObject(request.body());
    if(!json.has_value())
        return QHttpServerResponse(QHttpServerResponder::StatusCode::BadRequest);

    const QJsonObject obj = json.value();
    if(obj.contains("name") && obj.contains("subsystem") && obj.contains("type"))
    {
        const QString name = obj.value("name").toString();
        const QString subsystem = obj.value("subsystem").toString();
        const TagType type = Tag::typeFromString(obj.value("type").toString());
        const QString description = obj.value("description").toString("No description");

        if(name.isEmpty() || subsystem.isEmpty() || type == TagType::eUnknown)
        {
            return QHttpServerResponse(QHttpServerResponder::StatusCode::BadRequest);
        }

        switch (type) {
        case TagType::eBool:
        {
            auto value = obj.contains("value") ? obj.value("value").toBool() : false;
			tagList_.createTag(subsystem, name, TagType::eBool, value, description, false);
            break;
        }
        case TagType::eDouble:
        {
            auto value = obj.contains("value") ? obj.value("value").toDouble(0.0) : 0.0;
			tagList_.createTag(subsystem, name, TagType::eDouble, value, description, false);
            break;
        }
        case TagType::eInt:
        {
            auto value = obj.contains("value") ? obj.value("value").toInt() : 0;
			tagList_.createTag(subsystem, name, TagType::eInt, value, description, false);
            break;
        }
        case TagType::eString:
        {
            auto value = obj.contains("value") ? obj.value("value").toString("") : QString("");
			tagList_.createTag(subsystem, name, TagType::eString, value, description, false);
            break;
        }
            break;
        default:
            return QHttpServerResponse(QHttpServerResponder::StatusCode::BadRequest);
        }

        return QHttpServerResponse(QHttpServerResponder::StatusCode::Accepted);
    }

    return QHttpServerResponse(QHttpServerResponder::StatusCode::BadRequest);
}

// subsystem :
// name :
// value :
QHttpServerResponse TagApi::update(const QHttpServerRequest &request)
{
    const auto json = util::json::byteArrayToJsonObject(request.body());
    if(!json.has_value())
        return QHttpServerResponse(QHttpServerResponder::StatusCode::BadRequest);

    const QJsonObject obj = json.value();
    if(obj.contains("name") && obj.contains("subsystem") && obj.contains("value"))
    {
        const QString name = obj.value("name").toString();
        const QString subsystem = obj.value("subsystem").toString();

        auto *tag = tagList_.findByTagName(QString("%1.%2").arg(subsystem, name));
        if(!tag)
            return QHttpServerResponse(QHttpServerResponder::StatusCode::BadRequest);

        auto timestamp = QDateTime::currentMSecsSinceEpoch();
        QJsonValue value = obj.value("value");

        switch (tag->getType()) {
        case TagType::eBool:
        {
            tag->setValue(value.toBool(), timestamp);
            break;
        }
        case TagType::eDouble:
        {
			tag->setValue(value.toDouble(), timestamp);
			break;
        }
        case TagType::eInt:
        {
			tag->setValue(value.toInt(), timestamp);
			break;
        }
        case TagType::eString:
        {
            tag->setValue(value.toString(), timestamp);
            break;
        }
        default:
            return QHttpServerResponse(QHttpServerResponder::StatusCode::BadRequest);
        }

        return QHttpServerResponse(QHttpServerResponder::StatusCode::Accepted);
    }

    return QHttpServerResponse(QHttpServerResponder::StatusCode::BadRequest);
}

// subsystem :
// name :
QHttpServerResponse TagApi::get(const QHttpServerRequest &request)
{
    const auto json = util::json::byteArrayToJsonObject(request.body());
    if(!json.has_value())
        return QHttpServerResponse(QHttpServerResponder::StatusCode::BadRequest);

    const QJsonObject obj = json.value();
    if(obj.contains("name") && obj.contains("subsystem"))
    {
        const QString name = obj.value("name").toString();
        const QString subsystem = obj.value("subsystem").toString();

        auto *tag = tagList_.findByTagName(QString("%1.%2").arg(subsystem, name));
        if(tag)
            return QHttpServerResponse(tag->toJson());
    }

    return QHttpServerResponse(QHttpServerResponder::StatusCode::BadRequest);
}
