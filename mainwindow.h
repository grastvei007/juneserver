#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <memory>
#include <QDockWidget>
#include <QSystemTrayIcon>


namespace Ui {
class MainWindow;
}

class Menubar;
class Client;
class ClientListWidget;
class LoggerWidget;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void onNewConnection(Client *aclient);
    void onMinimizeToTray();
    void onRaiseWindow();

private slots:
    void onSystemTrayIconActivated(QSystemTrayIcon::ActivationReason reason);

private:
    Ui::MainWindow *ui;

    std::unique_ptr<Menubar> mMenubar;
    std::unique_ptr<QDockWidget> mClientListDockWidget;
    std::unique_ptr<QDockWidget> mLoggerDockWidget;

    std::unique_ptr<ClientListWidget> mclientListWidget;
    std::unique_ptr<LoggerWidget> mLoggerWidget;
    std::unique_ptr<QSystemTrayIcon> mSystemTrayIcon;
};

#endif // MAINWINDOW_H
