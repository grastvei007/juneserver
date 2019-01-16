#include "menubar.h"

#include <qt5/QtWidgets/qmenu.h>

Menubar::Menubar()
{
    QMenu *menu = new QMenu("File", this);

    addMenu(menu);
}

