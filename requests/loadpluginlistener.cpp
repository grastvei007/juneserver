#include "loadpluginlistener.h"

#include <QStringList>

LoadPluginListener::LoadPluginListener()
{

}

LoadPluginListener::LoadPluginListener(const QString &request, const Respons &respons, std::function<void(QString)> function) :
    RequestListener(request, respons),
    function_(function)
{

}

bool LoadPluginListener::isRequest(const QString &currentRequest)
{
    return currentRequest.startsWith(request());
}

Respons LoadPluginListener::executeRequest(QString command)
{
    QStringList list = command.split("/");
    QString pluginName = list.last();
    function_(pluginName);
    return respons();
}
