#ifndef AUTOMATIONMANAGER_H
#define AUTOMATIONMANAGER_H

#include <QObject>
#include <tagsystem/taglist.h>

#include <vector>
#include <memory>

#include "trigger.h"
#include "triggerfactory.h"

class QJsonArray;

class AutomationManager : public QObject
{
    Q_OBJECT
public:
    explicit AutomationManager(const QString& appName, TagList &taglist, QObject *parent = nullptr);

    bool createTrigger(const QJsonObject &obj);

    void removeTrigger(const QString &triggerName);
    void updateTrigger(const QJsonObject &obj);

    QJsonArray toJsonArray() const;

    void saveTriggers() const;
    void loadTriggers();

signals:
    void triggerCreated(QString);

private:
    QString appName_;
    TagList &tagList_;

    std::vector<std::unique_ptr<TriggerBase>> triggers_;
    TriggerFactory triggerFactory_;

    const QString triggerFile_ = {"triggers.json"};
};

#endif // AUTOMATIONMANAGER_H
