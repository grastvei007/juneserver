#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <qt5/QtWidgets/QMainWindow>
#include <memory>

namespace Ui {
class MainWindow;
}

class Menubar;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    std::unique_ptr<Menubar> mMenubar;
};

#endif // MAINWINDOW_H
