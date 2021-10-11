#ifndef LOADPLUGINLISTENER_H
#define LOADPLUGINLISTENER_H

#include "../rest/requestlistener.h"

#include <functional>

class LoadPluginListener : public RequestListener
{
public:
    LoadPluginListener();
    LoadPluginListener(const QString &request, const Respons &respons, std::function<void(QString)> function);

    bool isRequest(const QString &currentRequest) override;
    Respons executeRequest(QString command) override;

private:
    std::function<void(QString)> function_;
};

#endif // LOADPLUGINLISTENER_H
