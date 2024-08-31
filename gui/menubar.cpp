#include "menubar.h"

#include <QMenu>

Menubar::Menubar()
{
    mFileMenu = std::make_unique<QMenu>("File", this);

    addMenu(mFileMenu.get());

    mViewMenu = std::make_unique<QMenu>("View", this);

    mLogViewAction = std::make_unique<QAction>("Log View");
    connect(mLogViewAction.get(), &QAction::triggered, this, &Menubar::logViewTriggered);
    mViewMenu->addAction(mLogViewAction.get());

    tagsocketView_ = std::make_unique<QAction>("TagSocket View");
    connect(tagsocketView_.get(), &QAction::triggered, this, &Menubar::tagsocketViewTriggered);
    mViewMenu->addAction(tagsocketView_.get());

    addMenu(mViewMenu.get());
}


