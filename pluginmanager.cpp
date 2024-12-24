#include "pluginmanager.h"

#include <tagsystem/taglist.h>
#include <plugins/pluginload/pluginloader.h>
#include <QDebug>



void PluginManager::loadPlugin(const QString &path, const QString &name)
{
    auto plugin = pluginloader::load(path, name);
    if(!plugin)
    {
        qDebug() << "Error loading plugin " << QString("%1%2").arg(path, name);
        return;
    }

    plugin->setTagSystem(&TagList::sGetInstance());
    plugin->initialize();
    plugin->run(1000);

    plugins_.insert(name, plugin);
}
