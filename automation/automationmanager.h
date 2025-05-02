#ifndef AUTOMATIONMANAGER_H
#define AUTOMATIONMANAGER_H

#include <QObject>
#include <tagsystem/taglist.h>

#include <vector>
#include <memory>

#include "trigger.h"
#include "triggerfactory.h"

class AutomationManager : public QObject
{
    Q_OBJECT
public:
    explicit AutomationManager(TagList &taglist, QObject *parent = nullptr);

    bool createTrigger(const QJsonObject &obj);

signals:
    void triggerCreated(QString);

private:
    TagList &tagList_;

    std::vector<std::unique_ptr<TriggerBase>> triggers_;
    TriggerFactory triggerFactory_;
};

#endif // AUTOMATIONMANAGER_H
