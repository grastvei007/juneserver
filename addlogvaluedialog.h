#ifndef ADDLOGVALUEDIALOG_H
#define ADDLOGVALUEDIALOG_H

#include <QDialog>

class Tag;

namespace Ui {
class AddLogValueDialog;
}

class AddLogValueDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddLogValueDialog(QWidget *parent = nullptr);
    ~AddLogValueDialog();

    QString getTableName() const;
    QString getLogValueName() const;
    QString getTagSubSystem() const;
    QString getTagName() const;

private slots:
    void onSelectTagClicked(bool);

private:
    Ui::AddLogValueDialog *ui;
    Tag *mSelectedTag;
};

#endif // ADDLOGVALUEDIALOG_H
