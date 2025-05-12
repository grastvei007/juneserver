#include "app.h"

#include <QDebug>
#include <QCommandLineParser>
#include <QStringList>
#include <QtNetwork>
#include <QTimer>
#include <QSettings>

#include <tagsystem/taglistview.h>

#include "logvaluedata.h"
#include "logger.h"

#include "api/pluginapi.h"
#include "api/tagapi.h"

#ifdef NO_GUI
App::App(int argc, char *argv[]) : QCoreApplication(argc, argv)
#else
App::App(int argc, char *argv[]) : QApplication(argc, argv),
    mMainWindow(nullptr)
#endif
{
    mWebSocketServer = new WebSocketServer(5000, "JuneServer");
    setApplicationName("June Server");

    logValueData_ = new LogValueData(influxdb_);

    QCommandLineParser parser;
    parser.addHelpOption();
    QCommandLineOption noGui(QStringList() << "g" << "no-gui", "Gui" );
    parser.addOption(noGui);

    QCommandLineOption influxDbToken("influx", "InfluxDB Token for api v2", "token");
    parser.addOption(influxDbToken);

    QCommandLineOption influxDbEndPoint("endpoint", "InfluxDb endpoint", "ip");
    parser.addOption(influxDbEndPoint);

    parser.process(*this);

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

#ifndef NO_GUI
    if(parser.isSet(noGui))
    {
        mMainWindow = new MainWindow(logValueData_);
        mMainWindow->setWindowTitle("June Server");
        mMainWindow->setCentralWidget(new TagListView());
        mMainWindow->show();

        connect(mWebSocketServer, &WebSocketServer::newConnection, mMainWindow, &MainWindow::onNewConnection);
    }
    else
        connect(&Logger::sGetInstance(), &Logger::logEntry, this, &App::onLogEntry);
#else
    connect(&Logger::sGetInstance(), &Logger::logEntry, this, &App::onLogEntry);
#endif
    mSystemTimeTag = TagList::sGetInstance().createTag("system", "time", Tag::eTime);
    mSystemTimeTimer = new QTimer(this);
    mSystemTimeTimer->setInterval(1000);
    connect(mSystemTimeTimer, &QTimer::timeout, this, &App::onSystemTimeTimer);
    mSystemTimeTimer->start();

    loadPlugins();

    // start http server on port
    setupHttpServer(5005);
}

App::~App()
{
#ifndef NO_GUI
    if(mMainWindow)
        mMainWindow->deleteLater();
#endif
}


void App::onSystemTimeTimer()
{
    mSystemTimeTag->setValue(QDateTime::currentDateTime());
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

    /*httpServer_.setMissingHandler([](const QHttpServerRequest& request,
                                QHttpServerResponder &responder) {
        qDebug() << request.url();
    });*/

    pluginApi_ = std::make_unique<PluginApi>(httpServer_, pluginManager_);
    tagApi_ = std::make_unique<TagApi>(httpServer_, TagList::sGetInstance());

    tcpServer_ = std::make_unique<QTcpServer>();
    if(!tcpServer_->listen(QHostAddress::Any, port))
    {
        qDebug() << "Http server not running";
        return;
    }
    httpServer_.bind(tcpServer_.get());
}
