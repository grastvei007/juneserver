#ifndef MENUBAR_H
#define MENUBAR_H

#include <memory>

#include <QMenuBar>
#include <QMenu>
#include <QAction>

class Menubar : public QMenuBar
{
    Q_OBJECT
public:
    Menubar();

signals:
    void logViewTriggered(bool);

private:
    std::unique_ptr<QMenu> mViewMenu;
    std::unique_ptr<QMenu> mFileMenu;

    std::unique_ptr<QAction> mLogViewAction;
};

#endif // MENUBAR_H
