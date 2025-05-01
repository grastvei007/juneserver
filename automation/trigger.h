#ifndef TRIGGER_H
#define TRIGGER_H

#include <QObject>
#include <QJsonObject>

#include <tagsystem/tag.h>
#include <tagsystem/tagsocket.h>
#include <tagsystem/taglist.h>

enum class RuleType
{
    TriggerEveryTimeAbove,
    TriggerEveryTimeBelow,
    TriggerOnTime
};

class TriggerBase : public QObject
{
    Q_OBJECT
public:
    explicit TriggerBase(TagList &tagList, const QJsonObject &obj, QObject *parent = nullptr);

    bool isActive() const;

signals:

private:
    bool isActive_ = false;
    Tag *watchTag_;
    TagSocket *watchTagSocket_;
    TagList &tagList_;
};

#endif // TRIGGER_H
