#include "automationmanager.h"

#include <QJsonObject>
#include <QJsonArray>
#include <QDir>

#include <ranges>

#include <tagsystem/util/path.h>

#include "trigger/triggereverytimeabove.h"
#include "trigger/triggereverytimebelow.h"
#include "trigger/triggerontime.h"
#include "trigger/triggerschedule.h"
#include "trigger/triggertwovalues.h"


AutomationManager::AutomationManager(const QString &appName, TagList &taglist, QObject *parent)
    : appName_(appName)
    ,tagList_(taglist)
    , QObject(parent)
{
    triggerFactory_.addFactory<TriggerEveryTimeAbove>("triggerAbove");
    triggerFactory_.addFactory<TriggerEveryTimeBelow>("trigggerBelow");
    triggerFactory_.addFactory<TriggerOnTime>("triggerOnTime");
    triggerFactory_.addFactory<TriggerSchedule>("scheduleOnDuration");
    triggerFactory_.addFactory<TriggerTwoValues>("triggertwovalues");

    loadTriggers();
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
        connect(triggers_.back().get(),
                &TriggerBase::aboutToBeDestroyd,
                this,
                &AutomationManager::onTriggerAboutToBeDestroyd);
        emit triggerCreated(name);
        saveTriggers();
        return true;
    }

    return false;
}

TriggerBase *AutomationManager::findTriggerByName(const QString &triggerName)
{
    for (auto &trigger : triggers_)
    {
        if (trigger->triggerName() == triggerName)
            return trigger.get();
    }

    return nullptr;
}

void AutomationManager::removeTrigger(const QString &triggerName)
{
    if(std::erase_if(triggers_, [&triggerName](auto &trigger)
                      {return trigger->triggerName() == triggerName;}))
    {
        saveTriggers();
    }
}

void AutomationManager::updateTrigger(const QJsonObject &obj)
{
    for(auto &trigger : triggers_ | std::views::filter([&obj]
        (auto &element){return obj.value("triggername").toString() == element->triggerName();}))
    {
        trigger->update(obj);
        saveTriggers();
    }
}

QJsonArray AutomationManager::toJsonArray(bool allTriggers) const
{
    QJsonArray array;
    for (auto &trigger : triggers_ | std::views::filter([&allTriggers](auto &trigger) {
         if (allTriggers)
             return true;
         return trigger->shouldSave();
     }))
    {
        array.push_back(trigger->toJson());
    }
    return array;
}

void AutomationManager::saveTriggers() const
{
    if(triggers_.empty())
        return;

    QString configFile = util::path::configDir(appName_) + QDir::separator() + triggerFile_;
    QFile file(configFile);
    if(!file.open(QIODevice::WriteOnly))
    {
        qDebug() << "Error opening file: " << configFile;
        return;
    }

    QJsonObject obj;
    obj.insert("triggers", toJsonArray(false));
    QJsonDocument document(obj);

    QTextStream stream(&file);
    stream << document.toJson();
    file.close();
}

void AutomationManager::loadTriggers()
{
    QString configFile = util::path::configDir(appName_) + QDir::separator() + triggerFile_;
    QFile file(configFile);

    if(!file.exists())
        return;

    file.open(QIODevice::ReadOnly);
    QJsonDocument document = QJsonDocument::fromJson(file.readAll());

    if(document.isObject())
    {
        QJsonObject object = document.object();
        const QJsonArray triggers = object.value("triggers").toArray();
        for(const auto &triggerRef : triggers)
        {
            const QJsonObject &trigger = triggerRef.toObject();
            createTrigger(trigger);
        }
        qDebug() << "Triggers loaded: " << triggers.size();
    }
    file.close();
}

void AutomationManager::onTriggerAboutToBeDestroyd(const QString &triggerName)
{
    removeTrigger(triggerName);
}
