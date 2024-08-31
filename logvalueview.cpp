#include "logvalueview.h"
#include "ui_logvalueview.h"

#include <QPushButton>
#include <QLineEdit>

#include "logvaluemodel.h"
#include "addlogvaluedialog.h"

LogValueView::LogValueView(LogValueTableModel *aTableModel, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LogValueView),
    mTableModel(aTableModel)
{
    ui->setupUi(this);
    ui->tableView->setModel(mTableModel);

    connect(ui->addButton, &QPushButton::clicked, this, &LogValueView::onAddClicked);
    connect(ui->removeButton, &QPushButton::clicked, this, &LogValueView::onRemoveClicked);
    connect(ui->closeButton, &QPushButton::clicked, this, &LogValueView::onCloseClicked);
    connect(ui->saveButton, &QPushButton::clicked, this, &LogValueView::onSaveClicked);
}

LogValueView::~LogValueView()
{
    delete ui;
    delete mTableModel;
}

void LogValueView::onAddClicked(bool)
{
    AddLogValueDialog dialog;
    dialog.setVisible(true);
    if(dialog.exec() == QDialog::Accepted)
    {
        QString table = dialog.getTableName();
        QString valueName = dialog.getLogValueName();
        QString tagSubsystem = dialog.getTagSubSystem();
        QString tagName = dialog.getTagName();
        mTableModel->insertLogVallue(table, valueName, tagSubsystem, tagName);
    }
}

void LogValueView::onRemoveClicked(bool)
{

}

void LogValueView::onSaveClicked(bool)
{

}

void LogValueView::onCloseClicked(bool)
{
    hide();
}
