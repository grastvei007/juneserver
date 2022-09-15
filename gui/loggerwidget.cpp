#include "loggerwidget.h"

#include <QVBoxLayout>
#include <QLabel>
#include <QDebug>
#include <QScrollBar>
#include <QTime>
#include "logger.h"


LoggerWidget::LoggerWidget(QWidget *parent) : QWidget(parent)
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    QLabel *label = new QLabel(this);
    label->setText("Log");
    label->setVisible(true);
    layout->addWidget(label);

    mTextEdit.reset(new QPlainTextEdit());
    mTextEdit->document()->setMaximumBlockCount(20);
    layout->addWidget(mTextEdit.get());
    connect(&Logger::sGetInstance(), &Logger::logEntry, this, &LoggerWidget::onLogEntry);
}


void LoggerWidget::onLogEntry(QString message)
{
    mTextEdit->insertPlainText(message);
    QScrollBar *bar = mTextEdit->verticalScrollBar();
    bar->setValue(bar->maximum());
}
