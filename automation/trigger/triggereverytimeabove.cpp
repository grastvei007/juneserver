#include "triggereverytimeabove.h"

#include <tagsystem/tagsocketlist.h>

TriggerEveryTimeAbove::TriggerEveryTimeAbove(TagList &tagList, const QJsonObject &obj)
    : TriggerBase(tagList, obj)
{
    TriggerEveryTimeAbove::update(obj);
}

QJsonObject TriggerEveryTimeAbove::toJson() const
{
    auto json = TriggerBase::toJson();
    json.insert("triggervalue", triggerValue_);
    json.insert("duration", duration_);
    return json;
}

void TriggerEveryTimeAbove::update(const QJsonObject &obj)
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

TriggerType TriggerEveryTimeAbove::type() const
{
    return TriggerType::TriggerEveryTimeAbove;
}

void TriggerEveryTimeAbove::tagSocketValueChanged(TagSocket *tagSocket)
{
    if(!isValidTagSocketType(tagSocket))
        return;

    double tagSocketValue = getValueAsDouble(tagSocket);

    // set tag value true/false based on rule for this trigger type

    auto currentTime = QDateTime::currentSecsSinceEpoch();

        if( isActive() && currentTime > (triggerTime_ + duration_))
    {
        setDeactive();
    }

    if(tagSocketValue > triggerValue_)
    {
        if(!isActive())
        {
            triggerTime_ = currentTime;
            setActive();
        }
    }
}

bool TriggerEveryTimeAbove::isValidTagSocketType(TagSocket *tagSocket)
{
    return tagSocket->getType() != TagSocket::eDouble || tagSocket->getType() != TagSocket::eInt;
}

double TriggerEveryTimeAbove::getValueAsDouble(TagSocket *tagSocket)
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
