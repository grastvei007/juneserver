#ifndef TRIGGERSCHEDULE_H
#define TRIGGERSCHEDULE_H

#include "automation/trigger.h"

class TriggerSchedule : public TriggerBase
{
public:
    TriggerSchedule(TagList &tagList, const QJsonObject &obj);

    bool shouldSave() const final;
    QJsonObject toJson() const final;

    void update(const QJsonObject &obj) final;
    TriggerType type() const final;

protected:
    void tagSocketValueChanged(TagSocket *tagSocket) final;

private:
    bool originalTagValue_;
    qint64 duration_;
    bool scheduledValue_;
    qint64 startTime_ = -1;
    bool isDone_ = false;
};

#endif // TRIGGERSCHEDULE_H
