#include "logvalueview.h"
#include "ui_logvalueview.h"

#include "logvaluemodel.h"

LogValueView::LogValueView(LogValueTableModel *aTableModel, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LogValueView),
    mTableModel(aTableModel)
{
    ui->setupUi(this);
    ui->tableView->setModel(mTableModel);
}

LogValueView::~LogValueView()
{
    delete ui;
    delete mTableModel;
}
