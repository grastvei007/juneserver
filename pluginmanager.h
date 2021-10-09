#ifndef PLUGINMANAGER_H
#define PLUGINMANAGER_H

#include <map>
#include <string>

#include <plugins/pluginload/plugininterface.h>
#include <plugins/pluginload/pluginloader.h>

class PluginManager
{
public:
    PluginManager();

    void loadPlugin(std::string pluginName);
private:
    std::map<std::string, PluginInterface*> plugins_;
};

#endif // PLUGINMANAGER_H
