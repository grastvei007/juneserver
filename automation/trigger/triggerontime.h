#ifndef TRIGGERONTIME_H
#define TRIGGERONTIME_H

#include <QJsonObject>

#include "../trigger.h"

class TriggerOnTime : public TriggerBase
{
public:
    TriggerOnTime(TagList &tagList, const QJsonObject &obj);

    QJsonObject toJson() const override;

protected:
    void tagSocketValueChanged(TagSocket *tagSocket) override;

private:
    // seconds since midnight
    int triggerTimeOfDay_ = 0;
    // The time duration in minutes the trigger is active
    int duration_ = 0;
};

#endif // TRIGGERONTIME_H
