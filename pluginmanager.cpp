#include "pluginmanager.h"

#include <tagsystem/taglist.h>
#include <plugins/pluginload/pluginloader.h>
#include <QDebug>
#include <QPluginLoader>
#include <QProcessEnvironment>

PluginManager::PluginManager(QHttpServer &httpserver) :
    httpserver_(httpserver)
{

}

bool PluginManager::loadPlugin(const QString &pluginName)
{
    auto env = QProcessEnvironment::systemEnvironment();
    QString pluginPath = env.value("DEV_LIBS") + "/";

    return loadPlugin(pluginPath, pluginName);
}

bool PluginManager::loadPlugin(const QString &path, const QString &name)
{
    auto plugin = pluginloader::load(path, name);
    if(!plugin)
    {
        qDebug() << "Error loading plugin " << QString("%1%2").arg(path, name);
        return false;
    }

    plugin->setTagSystem(&TagList::sGetInstance());
    plugin->initialize();
    plugin->createApi(httpserver_);
    plugin->run(1000);

    plugins_.insert(name, plugin);
    return true;
}

bool PluginManager::unloadPlugin(const QString &name)
{
    if(!plugins_.contains(name))
        return false;

    auto plugin = plugins_[name];
    plugin->stop();

    QPluginLoader loader(plugin);
    loader.unload();
    plugins_.remove(name);
    return true;
}

QList<QString> PluginManager::runningPlugins() const
{
    return plugins_.keys();
}
