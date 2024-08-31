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
    void tagsocketViewTriggered(bool);

private:
    std::unique_ptr<QMenu> mViewMenu;
    std::unique_ptr<QMenu> mFileMenu;

    std::unique_ptr<QAction> mLogViewAction;
    std::unique_ptr<QAction> tagsocketView_;
};

#endif // MENUBAR_H
