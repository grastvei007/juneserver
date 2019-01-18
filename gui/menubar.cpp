#include "menubar.h"

#include <QMenu>

Menubar::Menubar()
{
    QMenu *menu = new QMenu("File", this);

    addMenu(menu);
}

