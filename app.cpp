#include "app.h"

#include <QDebug>
#include <QCommandLineParser>
#include <QStringList>
#include <QtNetwork>
#include <QTimer>
#include <QProcessEnvironment>
#include <QSettings>

#include <tagsystem/taglistview.h>

#include "logvaluedata.h"

#ifdef NO_GUI
App::App(int argc, char *argv[]) : QCoreApplication(argc, argv)
#else
App::App(int argc, char *argv[]) : QApplication(argc, argv),
    mMainWindow(nullptr)
#endif
{
    mWebSocketServer = new WebSocketServer(5000, "JuneServer");
    setApplicationName("June Server");

    mLogValueData = new LogValueData();

    QCommandLineParser parser;
    QCommandLineOption noGui(QStringList() << "g" << "no-gui", "Gui" );
    parser.addOption(noGui);

    parser.process(*this);
#ifndef NO_GUI
    if(parser.isSet(noGui))
    {
        mMainWindow = new MainWindow(mLogValueData);
        mMainWindow->setWindowTitle("June Server");
        mMainWindow->setCentralWidget(new TagListView());
        mMainWindow->show();

        connect(mWebSocketServer, &WebSocketServer::newConnection, mMainWindow, &MainWindow::onNewConnection);
    }
#endif
    mSystemTimeTag = TagList::sGetInstance().createTag("system", "time", Tag::eTime);
    mSystemTimeTimer = new QTimer(this);
    mSystemTimeTimer->setInterval(1000);
    connect(mSystemTimeTimer, &QTimer::timeout, this, &App::onSystemTimeTimer);
    mSystemTimeTimer->start();

    loadPlugins();
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

void App::loadPlugins()
{
    QProcessEnvironment env;
    QString value = env.value("DEV_LIBS");

    QSettings settings("june", "server");
    settings.beginGroup("plugins");
    if(settings.value("bms").toBool())
        pluginManager_.loadPlugin(QString("%1%2").arg(value, "bms").toStdString());
    if(settings.value("heaterd").toBool())
        pluginManager_.loadPlugin(QString("%1%2").arg(value, "heaterd").toStdString());
    if(settings.value("bmsd").toBool())
        pluginManager_.loadPlugin(QString("%1%2").arg(value, "bmsd").toStdString());

    if(settings.value("mcp3008").toBool())
        pluginManager_.loadPlugin(QString("%1%2").arg(value, "mcp3008").toStdString());
    else if(settings.value("mcp3008d").toBool())
        pluginManager_.loadPlugin(QString("%1%2").arg(value, "mcp3008d").toStdString());

    settings.endGroup();
}
