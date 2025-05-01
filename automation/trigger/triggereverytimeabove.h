#ifndef TRIGGEREVERYTIMEABOVE_H
#define TRIGGEREVERYTIMEABOVE_H

#include <QJsonObject>

#include "../trigger.h"

class TriggerEveryTimeAbove : public TriggerBase
{
public:
    TriggerEveryTimeAbove(TagList &tagList, const QJsonObject &obj);
};

#endif // TRIGGEREVERYTIMEABOVE_H
