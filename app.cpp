#include "app.h"

#include <QDebug>
#include <QCommandLineParser>
#include <QStringList>

#include <tagsystem/taglistview.h>

App::App(int argc, char *argv[]) : QApplication(argc, argv),
    mMainWindow(nullptr)
{
    mWebSocketServer = new WebSocketServer(5000, "JuneServer");
    setApplicationName("June Server");
    QCommandLineParser parser;
    QCommandLineOption gui(QStringList() << "g" << "gui", "Gui" );
    parser.addOption(gui);

    parser.process(*this);

    if(parser.isSet(gui))
    {
        mMainWindow = new MainWindow;
        mMainWindow->setWindowTitle("June Server");
        mMainWindow->setCentralWidget(new TagListView());
        mMainWindow->show();

    }
}

App::~App()
{
    if(mMainWindow)
        mMainWindow->deleteLater();
}


