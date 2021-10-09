#include "pluginmanager.h"

#include <tagsystem/taglist.h>

PluginManager::PluginManager()
{

}

void PluginManager::loadPlugin(std::string pluginName)
{
#ifndef _DEBUG
   // pluginName + "d";
#endif
    pluginName + ".so";
    PluginLoader loader;
    auto plugin = loader.load(QString::fromStdString(pluginName));
    if(!plugin)
        return;
    plugin->setTagSystem(&TagList::sGetInstance());
    plugin->initialize();
    plugin->run(1000);

    plugins_.insert(std::make_pair(pluginName, plugin));
}
