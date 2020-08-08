#include "logvalueview.h"
#include "ui_logvalueview.h"

LogValueView::LogValueView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LogValueView)
{
    ui->setupUi(this);
}

LogValueView::~LogValueView()
{
    delete ui;
}
