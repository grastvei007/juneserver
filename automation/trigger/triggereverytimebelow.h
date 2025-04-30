#ifndef TRIGGEREVERYTIMEBELOW_H
#define TRIGGEREVERYTIMEBELOW_H

#include <QJsonObject>

#include "../trigger.h"

class TriggerEveryTimeBelow : public TriggerBase
{
public:
    TriggerEveryTimeBelow(const QJsonObject &obj);
};

#endif // TRIGGEREVERYTIMEBELOW_H
