#include "triggerschedule.h"

#include <QJsonObject>

TriggerSchedule::TriggerSchedule(TagList &tagList, const QJsonObject &obj)
    : TriggerBase(tagList, obj)
{
    if (validateWatchTacksoket(TagSocket::eTime))
    {
        TriggerSchedule::update(obj);
        if (startTime_ < 0)
        {
            startTime_ = QDateTime::currentSecsSinceEpoch();
            setActive();
        }
    }
    else
    {
        emit aboutToBeDestroyd(triggerName());
    }

}

bool TriggerSchedule::shouldSave() const
{
    return false;
}

QJsonObject TriggerSchedule::toJson() const
{
    auto json = TriggerBase::toJson();
    json.insert("duration", duration_);
    json.insert("starttime", startTime_);
    return json;
}

void TriggerSchedule::update(const QJsonObject &obj)
{
    TriggerBase::update(obj);
    if (obj.contains("duration"))
    {
        duration_ = obj.value("duration").toInt();
    }
    // if trigger contains startime, calculate point the trigger should activate.
    if (obj.contains("starttime"))
    {
        startTime_ = QDateTime::currentSecsSinceEpoch() + obj.value("starttime").toInt();
    }
}

TriggerType TriggerSchedule::type() const
{
    return TriggerType::ScheduleOnDuration;
}

void TriggerSchedule::tagSocketValueChanged(TagSocket *tagSocket)
{
    if (!validateWatchTacksoket(TagSocket::eTime))
        return;

    // make sure it is not reactivated when it is deactivated
    if (isDone_)
        return;

    auto currentTime = QDateTime::currentSecsSinceEpoch();
    auto durationS = duration_ / 1000;

    if (!isActive())
    {
        if (currentTime > startTime_)
        {
            if(!isActive())
                setActive();
        }
        return;
    }

    if (isActive() && currentTime > (startTime_ + durationS))
    {
        setDeactive();
        isDone_ = true;
        emit aboutToBeDestroyd(triggerName());
    }
}
