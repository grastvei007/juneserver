#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <memory>
#include <QDockWidget>

namespace Ui {
class MainWindow;
}

class Menubar;
class Client;
class ClientListWidget;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void onNewConnection(Client *aclient);

private:
    Ui::MainWindow *ui;

    std::unique_ptr<Menubar> mMenubar;
    std::unique_ptr<QDockWidget> mClientListDockWidget;

    std::unique_ptr<ClientListWidget> mclientListWidget;
};

#endif // MAINWINDOW_H
