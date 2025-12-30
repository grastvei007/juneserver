#ifndef PLUGINMANAGER_H
#define PLUGINMANAGER_H

#include <QMap>
#include <QString>
#include <QList>

#include <plugins/pluginload/plugininterface.h>

class QHttpServer;

class PluginManager : public QObject
{
    Q_OBJECT
public:
    explicit PluginManager(QHttpServer &httpserver);

    bool loadPlugin(const QString& pluginName);
    bool loadPlugin(const QString &path, const QString &name);

    bool unloadPlugin(const QString& name);

    QList<QString> runningPlugins() const;
private:
    QMap<QString, PluginInterface*> plugins_;
    QHttpServer &httpserver_;
    QString pluginPath_;
};

#endif // PLUGINMANAGER_H
