#include "automationmanager.h"

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
