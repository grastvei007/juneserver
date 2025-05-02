#include "automationmanager.h"

#include <QJsonObject>

#include "trigger/triggereverytimeabove.h"
#include "trigger/triggereverytimebelow.h"
#include "trigger/triggerontime.h"

AutomationManager::AutomationManager(TagList &taglist, QObject *parent) :
    tagList_(taglist),
    QObject(parent)
{
    triggerFactory_.addFactory<TriggerEveryTimeAbove>("triggerAbove");
    triggerFactory_.addFactory<TriggerEveryTimeBelow>("trigggerBelow");
    triggerFactory_.addFactory<TriggerOnTime>("triggerOnTime");
}

bool AutomationManager::createTrigger(const QJsonObject &obj)
{
    if(!obj.contains("type"))
        return false;

    const QString key = obj.value("type").toString();

    auto trigger = triggerFactory_.createTrigger(key, tagList_, obj);
    if(trigger && !trigger->triggerName().isEmpty())
    {
        QString name = trigger->triggerName();
        triggers_.emplace_back(std::unique_ptr<TriggerBase>(trigger));
        emit triggerCreated(name);
        return true;
    }

    return false;
}
