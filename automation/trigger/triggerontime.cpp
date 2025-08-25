#include "triggerontime.h"

TriggerOnTime::TriggerOnTime(TagList &tagList, const QJsonObject &obj)
    : TriggerBase(tagList, obj)
{
    TriggerOnTime::update(obj);

}

QJsonObject TriggerOnTime::toJson() const
{
    auto json = TriggerBase::toJson();
    json.insert("triggervalue", triggerTimeOfDay_);
    json.insert("duration", duration_);
    return json;
}

void TriggerOnTime::update(const QJsonObject &obj)
{
    TriggerBase::update(obj);
    if(obj.contains("triggervalue"))
    {
        triggerTimeOfDay_ = obj.value("triggervalue").toInt();
    }
    if(obj.contains("duration"))
    {
        duration_ = obj.value("duration").toInt();
    }
}

TriggerType TriggerOnTime::type() const
{
    return TriggerType::TriggerOnTime;
}

void TriggerOnTime::tagSocketValueChanged(TagSocket *tagSocket)
{
    if(tagSocket->getType() != TagSocket::eTime)
        return;

    QDateTime value;
    if(!tagSocket->readValue(value))
        return;

    int tagSocketTimeOfDayValue = value.time().msecsSinceStartOfDay() * 1000;

    if(tagSocketTimeOfDayValue > triggerTimeOfDay_)
    {
        if(!isActive())
            setActive();
    }

    if(isActive() && duration_ > 0 && tagSocketTimeOfDayValue > (triggerTimeOfDay_ + duration_))
    {
        setDeactive();
    }
}
