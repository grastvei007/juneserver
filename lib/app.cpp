#include "app.h"

#include <QDebug>
#include <QCommandLineParser>
#include <QStringList>
#include <QtNetwork>
#include <QTimer>
#include <QSettings>

#include <tagsystem/taglistview.h>
#include <tagsystem/tagsocketlist.h>
#include <tagsystem/util/json.h>

#include "logvaluedata.h"
#include "logger.h"

#include "api/pluginapi.h"
#include "api/tagapi.h"
#include "api/triggerapi.h"
#include "api/tagsocketbindingapi.h"
#include "api/logvalueapi.h"


App::App(int argc, char *argv[]) : QCoreApplication(argc, argv)
{   const QString appName("JuneServer");
    // Setup tagsocket list to load all tagsockets at startup.
    TagSocketList::sGetInstance().setApplicationName(appName);
    TagSocketList::sGetInstance().loadBindingList();
    TagSocketList::sGetInstance().setAutoSave();

    mWebSocketServer = new WebSocketServer(5000, appName);
    setApplicationName(appName);

    logValueData_ = new LogValueData(appName, influxdb_);

    QCommandLineParser parser;
    parser.addHelpOption();

    QCommandLineOption influxDbToken("influx", "InfluxDB Token for api v2", "token");
    parser.addOption(influxDbToken);

    QCommandLineOption influxDbEndPoint("endpoint", "InfluxDb endpoint", "ip");
    parser.addOption(influxDbEndPoint);

    QCommandLineOption tagFile("tagfile",
                               "Load a file with tags on startup, make the server ready for testing with user spessified tags.",
                               "file");
    parser.addOption(tagFile);

    parser.addOption({{"d", "debug"}, "Debug mode. Print info when a tag value is updated."});

    parser.process(*this);

    if (parser.isSet("debug"))
    {
        mWebSocketServer->setDebugTagValues();
    }

    if (parser.isSet(tagFile))
    {
        auto filename = parser.value(tagFile);
        loadTagFile(filename);
    }

    if(parser.isSet(influxDbEndPoint))
    {
        auto ip = parser.value(influxDbEndPoint);
        influxdb_.setAdress(ip);
    }

    if(parser.isSet(influxDbToken))
    {
        // if token is passed it is to use influxDb v2.
        // add token, and setup the url for the db.
        auto token = parser.value(influxDbToken).toUtf8();
        influxdb_.setApiToken(token);
        influxdb_.getBuckets("june");
    }
    else
    {
        influxdb_.useDb("june");
    }

    connect(&Logger::sGetInstance(), &Logger::logEntry, this, &App::onLogEntry);

    mSystemTimeTag = TagList::sGetInstance().createTag("system", "time", TagType::eTime);
    mSystemTimeTimer = new QTimer(this);
    mSystemTimeTimer->setInterval(1000);
    connect(mSystemTimeTimer, &QTimer::timeout, this, &App::onSystemTimeTimer);
    mSystemTimeTimer->start();

    loadPlugins();

    automationManager_ = std::make_unique<AutomationManager>(appName, TagList::sGetInstance());

    // start http server on port
    setupHttpServer(5005);
}

App::~App()
{

}


void App::onSystemTimeTimer()
{
    mSystemTimeTag->setValue(QDateTime::currentDateTime().toMSecsSinceEpoch());
}

void App::onLogEntry(QString message)
{
    qDebug() << message;
}

void App::loadPlugins()
{
    QSettings settings("june", "server");
    settings.beginGroup("plugins");
    for(const auto &pluginName : settings.childKeys())
    {
        auto isLodingPlugin = settings.value(pluginName).toBool();
        if(isLodingPlugin)
            pluginManager_.loadPlugin(pluginName);
    }
}

void App::setupHttpServer(quint16 port)
{
    // setup all routes on httpserver
    httpServer_.route("/", []() {
        return "June rest api up an running";
    });

    httpServer_.setMissingHandler(this, [](const QHttpServerRequest& request,
                                QHttpServerResponder &responder) {
        qDebug() << request.url();
    });

    pluginApi_ = std::make_unique<PluginApi>(httpServer_, pluginManager_);
    tagApi_ = std::make_unique<TagApi>(httpServer_, TagList::sGetInstance());
    triggerApi_ = std::make_unique<TriggerApi>(httpServer_, *automationManager_.get());
    tagSocketBindingApi_ = std::make_unique<TagSocketBindingApi>(httpServer_, TagSocketList::sGetInstance());
    logValueApi_ = std::make_unique<LogValueApi>(httpServer_, *logValueData_);

    tcpServer_ = std::make_unique<QTcpServer>();
    if(!tcpServer_->listen(QHostAddress::Any, port))
    {
        qDebug() << "Http server not running";
        return;
    }
    httpServer_.bind(tcpServer_.get());
}

void App::loadTagFile(const QString &filename)
{
    QFile file(filename);
    if (!file.exists())
    {
        qDebug() << "Try to load a tag file, that does not exists, " << filename;
        return;
    }
    if (!file.open(QIODevice::ReadOnly))
    {
        qDebug() << "Error opening tag file, " << filename;
    }

    auto json = util::json::byteArrayToJsonArray(file.readAll());
    if (!json.has_value())
    {
        qDebug() << "Invalid json format in file, " << filename;
        return;
    }

    const QJsonArray array = json.value();
    for (const auto &ref : array)
    {
        const QJsonObject &obj = ref.toObject();
        TagList::sGetInstance().UpdateOrCreateTag(obj);
    }
    qDebug() << "Preload with " << array.size() << " tags.";
}
