#ifndef PLUGINMANAGER_H
#define PLUGINMANAGER_H

#include <QMap>
#include <QString>

#include <plugins/pluginload/plugininterface.h>

class PluginManager : public QObject
{
    Q_OBJECT
public:
    explicit PluginManager() = default;

    bool loadPlugin(const QString& pluginName);
    bool loadPlugin(const QString &path, const QString &name);

    bool unloadPlugin(const QString& name);
private:
    QMap<QString, PluginInterface*> plugins_;
};

#endif // PLUGINMANAGER_H
