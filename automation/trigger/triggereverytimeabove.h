#ifndef TRIGGEREVERYTIMEABOVE_H
#define TRIGGEREVERYTIMEABOVE_H

#include <QJsonObject>

#include "../trigger.h"

class TriggerEveryTimeAbove : public TriggerBase
{
public:
    TriggerEveryTimeAbove(TagList &tagList, const QJsonObject &obj);

protected:
    void tagSocketValueChanged(TagSocket *tagSocket) override;

private:
    bool isValidTagSocketType(TagSocket *tagSocket);
    double getValueAsDouble(TagSocket *tagSocket);

    double triggerValue_ = 0.;
};

#endif // TRIGGEREVERYTIMEABOVE_H
