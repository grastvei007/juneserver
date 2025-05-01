#ifndef TRIGGERONTIME_H
#define TRIGGERONTIME_H

#include <QJsonObject>

#include "../trigger.h"

class TriggerOnTime : public TriggerBase
{
public:
    TriggerOnTime(TagList &tagList, const QJsonObject &obj);
};

#endif // TRIGGERONTIME_H
