#include "app.h"

#include <functional>

#include <QDebug>
#include <QCommandLineParser>
#include <QStringList>
#include <QtNetwork>
#include <QTimer>
#include <QProcessEnvironment>

#include <tagsystem/taglistview.h>

#include "logvaluedata.h"

#include "requests/loadpluginlistener.h"


App::App(int argc, char *argv[]) : QApplication(argc, argv),
    mMainWindow(nullptr)
{
    mWebSocketServer = new WebSocketServer(5000, "JuneServer");
    setApplicationName("June Server");

    mLogValueData = new LogValueData();

    QCommandLineParser parser;
    QCommandLineOption noGui(QStringList() << "g" << "no-gui", "Gui" );
    parser.addOption(noGui);

    parser.process(*this);

    if(parser.isSet(noGui))
    {
        mMainWindow = new MainWindow(mLogValueData);
        mMainWindow->setWindowTitle("June Server");
        mMainWindow->setCentralWidget(new TagListView());
        mMainWindow->show();

        connect(mWebSocketServer, &WebSocketServer::newConnection, mMainWindow, &MainWindow::onNewConnection);

    }
    mSystemTimeTag = TagList::sGetInstance().createTag("system", "time", Tag::eTime);
    mSystemTimeTimer = new QTimer(this);
    mSystemTimeTimer->setInterval(1000);
    connect(mSystemTimeTimer, &QTimer::timeout, this, &App::onSystemTimeTimer);
    mSystemTimeTimer->start();

    loadPlugins();

    restServer_.reset(new RestServer);
    restServer_->listen(8080);
    setupRestResponses();
}

App::~App()
{
    if(mMainWindow)
        mMainWindow->deleteLater();
}


void App::onSystemTimeTimer()
{
    mSystemTimeTag->setValue(QDateTime::currentDateTime());
}

void App::loadPlugin(QString name)
{
    qDebug() << "load plugin, " << name;
    QProcessEnvironment env;
    QString value = env.value("DEV_LIBS");
    pluginManager_.loadPlugin(QString("%1%2").arg(value).arg(name).toStdString());
}

void App::setupRestResponses()
{
    Respons res;
    std::function<void(QString)> function = std::bind(&App::loadPlugin, this, std::placeholders::_1);
    LoadPluginListener *lpl = new LoadPluginListener("/loadplugin/", res, function);
    restServer_->addRequestListener(lpl);
}

void App::loadPlugins()
{
    QProcessEnvironment env;
    QString value = env.value("DEV_LIBS");


    pluginManager_.loadPlugin(QString("%1%2").arg(value).arg("bms").toStdString());
 //   pluginManager_.loadPlugin(QString("%1%2").arg(value).arg("heaterd").toStdString());
}
