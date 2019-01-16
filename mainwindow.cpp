#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "gui/menubar.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    mMenubar.reset(new Menubar());
    setMenuBar(mMenubar.get());
}

MainWindow::~MainWindow()
{
    delete ui;
}
