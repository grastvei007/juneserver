#ifndef LOGGERWIDGET_H
#define LOGGERWIDGET_H

#include <QWidget>
#include <QPlainTextEdit>
#include <memory>

class LoggerWidget : public QWidget
{
    Q_OBJECT
public:
    explicit LoggerWidget(QWidget *parent = nullptr);

signals:

private slots:
    void onLogEntry(QString aMsg);

private:
    std::unique_ptr<QPlainTextEdit> mTextEdit;
};

#endif // LOGGERWIDGET_H
