#include "pluginmanager.h"

#include <tagsystem/taglist.h>
#include <plugins/pluginload/pluginloader.h>
#include <QDebug>


void PluginManager::loadPlugin(std::string pluginName)
{
#ifndef _DEBUG
   // pluginName + "d";
#endif
    //pluginName += ".so";
    qDebug() << pluginName;
    PluginLoader *loader = new PluginLoader();
    auto plugin = loader->load(QString::fromStdString(pluginName));
    if(!plugin)
        return;
    plugin->setTagSystem(&TagList::sGetInstance());
    plugin->initialize();
    plugin->run(1000);

    plugins_.insert(std::make_pair(pluginName, plugin));
}
