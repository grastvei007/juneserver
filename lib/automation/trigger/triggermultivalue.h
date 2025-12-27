#ifndef TRIGGERMULTIVALUE_H
#define TRIGGERMULTIVALUE_H

#include <QJsonObject>

#include "../trigger.h"

class TriggerMultiValue : public TriggerBase
{
public:
    TriggerMultiValue(TagList &tagList, const QJsonObject &obj);

    void update(const QJsonObject &obj) override;
};

#endif // TRIGGERMULTIVALUE_H
