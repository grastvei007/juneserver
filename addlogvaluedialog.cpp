#include "addlogvaluedialog.h"
#include "ui_addlogvaluedialog.h"

#include <tagsystem/tagselectview.h>
#include <tagsystem/tag.h>

AddLogValueDialog::AddLogValueDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddLogValueDialog),
    mSelectedTag(nullptr)
{
    ui->setupUi(this);
    connect(ui->selectTagButton, &QPushButton::clicked, this, &AddLogValueDialog::onSelectTagClicked);
}

AddLogValueDialog::~AddLogValueDialog()
{
    delete ui;
}

QString AddLogValueDialog::getTableName() const
{
    return ui->tableNameLineEdit->text();
}

QString AddLogValueDialog::getLogValueName() const
{
    return ui->valueNameLineEdit->text();
}

QString AddLogValueDialog::getTagSubSystem() const
{
    if(mSelectedTag)
        return mSelectedTag->getSubsystem();
    return QString();
}

QString AddLogValueDialog::getTagName() const
{
    if(mSelectedTag)
        return mSelectedTag->getName();
    return QString();
}

void AddLogValueDialog::onSelectTagClicked(bool)
{
    TagSelectView tagSelectView;
    tagSelectView.setVisible(true);
    if(tagSelectView.exec() == QDialog::Accepted)
    {
        mSelectedTag = tagSelectView.getSelectedTag();
        if(mSelectedTag)
        {
            ui->TagNameLineEdit->clear();
            ui->TagNameLineEdit->setText(mSelectedTag->getFullName());
        }
    }
}
