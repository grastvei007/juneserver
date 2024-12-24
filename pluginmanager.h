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

    void loadPlugin(const QString &path, const QString &name);
private:
    QMap<QString, PluginInterface*> plugins_;
};

#endif // PLUGINMANAGER_H
