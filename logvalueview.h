#ifndef LOGVALUEVIEW_H
#define LOGVALUEVIEW_H

#include <QWidget>

namespace Ui {
class LogValueView;
}

class LogValueView : public QWidget
{
    Q_OBJECT

public:
    explicit LogValueView(QWidget *parent = nullptr);
    ~LogValueView();

private:
    Ui::LogValueView *ui;
};

#endif // LOGVALUEVIEW_H
