#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "gui/menubar.h"
#include "gui/clientlistwidget.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    mMenubar.reset(new Menubar());
    setMenuBar(mMenubar.get());

    mclientListWidget.reset(new ClientListWidget(this));

    mClientListDockWidget.reset(new QDockWidget());
    mClientListDockWidget->setWidget(mclientListWidget.get());
    addDockWidget(Qt::LeftDockWidgetArea, mClientListDockWidget.get());

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::onNewConnection(Client *aClient)
{
    mclientListWidget->addClient(aClient);
}
