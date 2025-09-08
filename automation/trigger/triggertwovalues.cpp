#include "triggertwovalues.h"

TriggerTwoValues::TriggerTwoValues(TagList &tagList, const QJsonObject &obj, QObject *parent)
    : TriggerBase(tagList, obj, parent)
{
    TriggerTwoValues::update(obj);
}

void TriggerTwoValues::update(const QJsonObject &obj)
{
    TriggerBase::update(obj);

    // allow changes to trigger behavior, not only values
    triggerOnBelowValue_ = false;
    triggerOnAboveValue_ = false;
    triggerOffBelowValue_ = false;
    triggerOffAboveValue_ = false;

    if (obj.contains("triggeronbelowvalue"))
    {
        triggerOnBelowValue_ = true;
        valueTriggerOn_ = obj.value("triggeronbelowvalue").toDouble();
    }
    if (obj.contains("triggeronabovevalue"))
    {
        triggerOnAboveValue_ = true;
        valueTriggerOn_ = obj.value("triggeronabovevalue").toDouble();
    }
    if (obj.contains("triggeroffbelowvalue"))
    {
        triggerOffBelowValue_ = true;
        valueTriggerOff_ = obj.value("triggeroffbelowvalue").toDouble();
    }
    if (obj.contains("triggeroffabovevalue"))
    {
        triggerOffAboveValue_ = true;
        valueTriggerOff_ = obj.value("triggeroffabovevalue").toDouble();
    }
}

QJsonObject TriggerTwoValues::toJson() const
{
    auto json = TriggerBase::toJson();
    if (triggerOnBelowValue_)
        json.insert("triggeronbelowvalue", valueTriggerOn_);
    if (triggerOnAboveValue_)
        json.insert("triggeronabovevalue", valueTriggerOn_);
    if (triggerOffBelowValue_)
        json.insert("triggeroffbelowvalue", valueTriggerOff_);
    if (triggerOffAboveValue_)
        json.insert("triggeroffabovevalue", valueTriggerOff_);

    return json;
}

TriggerType TriggerTwoValues::type() const
{
    return TriggerType::TriggerTwoValues;
}

void TriggerTwoValues::tagSocketValueChanged(TagSocket *tagSocket)
{
    if (!isValidTagSocketType(tagSocket))
        return;

    double tagSocketValue = getValueAsDouble(tagSocket);

    if (isActive())
    {
        if (triggerOffAboveValue_)
        {
            if (tagSocketValue > valueTriggerOff_)
                setDeactive();
        } else if (triggerOffBelowValue_)
        {
            if (tagSocketValue < valueTriggerOff_)
                setDeactive();
        }
    } else
    {
        if (triggerOnAboveValue_)
        {
            if (tagSocketValue > valueTriggerOn_)
                setActive();
        } else if (triggerOnBelowValue_)
        {
            if (tagSocketValue < valueTriggerOn_)
                setActive();
        }
    }
}

bool TriggerTwoValues::isValidTagSocketType(TagSocket *tagSocket)
{
    return tagSocket->getType() != TagSocket::eDouble || tagSocket->getType() != TagSocket::eInt;
}

double TriggerTwoValues::getValueAsDouble(TagSocket *tagSocket)
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
