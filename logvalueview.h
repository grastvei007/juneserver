#ifndef LOGVALUEVIEW_H
#define LOGVALUEVIEW_H

#include <QWidget>

class LogValueTableModel;

namespace Ui {
class LogValueView;
}

class LogValueView : public QWidget
{
    Q_OBJECT

public:
    explicit LogValueView(LogValueTableModel *aTableModel, QWidget *parent = nullptr);
    ~LogValueView();

private:
    Ui::LogValueView *ui;

    LogValueTableModel *mTableModel;
};

#endif // LOGVALUEVIEW_H
