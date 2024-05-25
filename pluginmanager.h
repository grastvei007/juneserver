#ifndef PLUGINMANAGER_H
#define PLUGINMANAGER_H

#include <map>
#include <string>

#include <plugins/pluginload/plugininterface.h>

class PluginManager : public QObject
{
    Q_OBJECT
public:
    explicit PluginManager() = default;

    void loadPlugin(std::string pluginName);
private:
    std::map<std::string, PluginInterface*> plugins_;
};

#endif // PLUGINMANAGER_H
