#include "trigger.h"

TriggerBase::TriggerBase(const QJsonObject &obj, QObject *parent)
    : QObject{parent}
{}

bool TriggerBase::isActive() const
{
    return isActive_;
}
