#include "app.h"

#include <QDebug>
#include <QCommandLineParser>
#include <QStringList>
#include <QtNetwork>
#include <QTimer>

#include <tagsystem/taglistview.h>
#include <plugins/pluginload/plugininterface.h>
#include <plugins/pluginload/pluginloader.h>

#include "logvaluedata.h"

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

void App::loadPlugins()
{
    PluginLoader loader;
    auto plugin = loader.load("bms");

    plugin->setTagSystem(&TagList::sGetInstance());
    plugin->initialize();
}
