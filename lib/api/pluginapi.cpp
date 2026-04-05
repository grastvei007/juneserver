#include "pluginapi.h"

#include "pluginmanager.h"

#include <QHttpServer>
#include <QHttpServerRequest>
#include <QHttpServerResponse>

#include <tagsystem/util/json.h>
#include <QJsonObject>
#include <QJsonArray>
#include <QtConcurrent/qtconcurrentrun.h>
#include <optional>

PluginApi::PluginApi(QHttpServer &httpServer, PluginManager &pluginManager) :
    httpServer_(httpServer),
    pluginManager_(pluginManager)
{
    httpServer.route("/api/plugin", QHttpServerRequest::Method::Get,
            [this](const QHttpServerRequest &request) { return getRunningPlugins(request); });

    httpServer_.route( "/api/plugin/load", QHttpServerRequest::Method::Post,
            [this](const QHttpServerRequest &request) { return loadPluginRequest(request); });

    httpServer_.route( "/api/plugin/unload", QHttpServerRequest::Method::Post,
            [this](const QHttpServerRequest &request) { return unloadPluginRequest(request); });
}

QHttpServerResponse PluginApi::loadPluginRequest(const QHttpServerRequest &request)
{
    const auto json = util::json::byteArrayToJsonObject(request.body());
    if(!json.has_value())
        qDebug() << __FUNCTION__ << request.body();

    if(json.has_value() && json->contains("name"))
    {
        auto pluginName = json->value("name").toString();
        if(pluginManager_.loadPlugin(pluginName))
            return QHttpServerResponse(QHttpServerResponder::StatusCode::Ok);
    }

    return QHttpServerResponse(QHttpServerResponder::StatusCode::BadRequest);
}

QHttpServerResponse PluginApi::unloadPluginRequest(const QHttpServerRequest &request)
{
    const auto json = util::json::byteArrayToJsonObject(request.body());

    if(json.has_value() && json->contains("name"))
    {
        auto pluginName = json->value("name").toString();
        if(pluginManager_.unloadPlugin(pluginName))
            return QHttpServerResponse(QHttpServerResponder::StatusCode::Ok);
    }

    return QHttpServerResponse(QHttpServerResponder::StatusCode::BadRequest);
}

QHttpServerResponse PluginApi::getRunningPlugins(const QHttpServerRequest &request)
{
	const auto plugins = pluginManager_.runningPlugins();

	auto jsonArray = QJsonArray();
    for(const auto& plugin : plugins)
    {
        auto jsonObject = QJsonValue{{"name", plugin}};
        jsonArray.append(jsonObject);
    }

    return !jsonArray.isEmpty() ? QHttpServerResponse(jsonArray)
        : QHttpServerResponse(QHttpServerResponder::StatusCode::NoContent);
}
