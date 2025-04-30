#ifndef AUTOMATIONMANAGER_H
#define AUTOMATIONMANAGER_H

#include <QObject>
#include <tagsystem/taglist.h>

#include <map>
#include <memory>

#include "trigger.h"
#include "triggerfactory.h"

class AutomationManager : public QObject
{
    Q_OBJECT
public:
    explicit AutomationManager(TagList &taglist, QObject *parent = nullptr);

signals:

private:
    TagList &tagList_;

    std::map<int, std::unique_ptr<TriggerBase>> rules_;
    TriggerFactory triggerFactory_;
};

#endif // AUTOMATIONMANAGER_H
