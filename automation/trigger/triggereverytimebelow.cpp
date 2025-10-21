#include "triggereverytimebelow.h"

TriggerEveryTimeBelow::TriggerEveryTimeBelow(TagList &tagList, const QJsonObject &obj)
    : TriggerBase(tagList, obj)
{
    TriggerEveryTimeBelow::update(obj);
}

QJsonObject TriggerEveryTimeBelow::toJson() const
{
    auto json = TriggerBase::toJson();
    json.insert("triggervalue", triggerValue_);
    json.insert("duration", duration_);
    return json;
}

void TriggerEveryTimeBelow::update(const QJsonObject &obj)
{
    TriggerBase::update(obj);
    if(obj.contains("triggervalue"))
    {
        triggerValue_ = obj.value("triggervalue").toDouble();
    }
    if (obj.contains("duration"))
    {
        duration_ = obj.value("duration").toInt();
    }
}

TriggerType TriggerEveryTimeBelow::type() const
{
    return TriggerType::TriggerEveryTimeBelow;
}

void TriggerEveryTimeBelow::tagSocketValueChanged(TagSocket *tagSocket)
{
    if(!isValidTagSocketType(tagSocket))
        return;

    double tagSocketValue = getValueAsDouble(tagSocket);

    // set tag value true/false based on rule for this trigger type

    auto currentTime = QDateTime::currentSecsSinceEpoch();
    auto durationS = duration_ / 1000;

    if( isActive() && currentTime > (triggerTime_ + durationS))
    {
        if(isActive())
            setDeactive();
    }

    if(tagSocketValue < triggerValue_)
    {
        if(!isActive())
        {
            triggerTime_ = currentTime;
            setActive();
        }
    }
}

bool TriggerEveryTimeBelow::isValidTagSocketType(TagSocket *tagSocket)
{
    return tagSocket->getType() != TagSocket::eDouble || tagSocket->getType() != TagSocket::eInt;
}

double TriggerEveryTimeBelow::getValueAsDouble(TagSocket *tagSocket)
{
    if(tagSocket->getType() == TagSocket::eDouble)
    {
        double value;
        if(tagSocket->readValue(value))
            return value;
    }
    else if(tagSocket->getType() == TagSocket::eInt)
    {
        int value;
        if(tagSocket->readValue(value))
            return static_cast<double>(value);
    }

    // should never happen
    return 0.0;
}
