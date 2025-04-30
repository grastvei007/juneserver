#ifndef TRIGGER_H
#define TRIGGER_H

#include <QObject>
#include <QJsonObject>

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
    explicit TriggerBase(const QJsonObject &obj, QObject *parent = nullptr);

    bool isActive() const;

signals:

private:
    bool isActive_ = false;
};

#endif // TRIGGER_H
