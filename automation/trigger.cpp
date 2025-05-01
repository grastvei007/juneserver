#include "trigger.h"

TriggerBase::TriggerBase(TagList &tagList, const QJsonObject &obj, QObject *parent) :
    tagList_(tagList),
    QObject{parent}
{

}

bool TriggerBase::isActive() const
{
    return isActive_;
}
