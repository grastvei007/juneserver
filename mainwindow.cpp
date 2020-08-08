#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "gui/menubar.h"
#include "gui/clientlistwidget.h"
#include "gui/loggerwidget.h"
#include "logvalueview.h"



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    mMenubar.reset(new Menubar());
    setMenuBar(mMenubar.get());
    connect(mMenubar.get(), &Menubar::logViewTriggered, this, &MainWindow::onLogValueViewTriggered);

    mSystemTrayIcon.reset(new QSystemTrayIcon(QIcon(":/icon")));
    mSystemTrayIcon->setVisible(true);
    connect(mSystemTrayIcon.get(), &QSystemTrayIcon::activated, this, &MainWindow::onSystemTrayIconActivated);


    mclientListWidget.reset(new ClientListWidget(this));

    mClientListDockWidget.reset(new QDockWidget());
    mClientListDockWidget->setWidget(mclientListWidget.get());
    addDockWidget(Qt::LeftDockWidgetArea, mClientListDockWidget.get());

    mLoggerWidget.reset(new LoggerWidget(this));
    mLoggerDockWidget.reset(new QDockWidget());
    mLoggerDockWidget->setWidget(mLoggerWidget.get());
    addDockWidget(Qt::BottomDockWidgetArea, mLoggerDockWidget.get());

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::onNewConnection(Client *aClient)
{
    mclientListWidget->addClient(aClient);
}


void MainWindow::onMinimizeToTray()
{

}


void MainWindow::onRaiseWindow()
{

}


void MainWindow::onSystemTrayIconActivated(QSystemTrayIcon::ActivationReason reason)
{
    if(reason == QSystemTrayIcon::DoubleClick)
    {

        raise();
    }
}

void MainWindow::onLogValueViewTriggered(bool)
{
    if(mLogValueWidget == nullptr)
    {
        mLogValueWidget.reset(new LogValueView());
    }
    mLogValueWidget->setVisible(true);
    mLogValueWidget->raise();
}
