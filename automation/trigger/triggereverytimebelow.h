#ifndef TRIGGEREVERYTIMEBELOW_H
#define TRIGGEREVERYTIMEBELOW_H

#include <QJsonObject>

#include "../trigger.h"

class TriggerEveryTimeBelow : public TriggerBase
{
public:
    TriggerEveryTimeBelow(TagList &tagList, const QJsonObject &obj);

    QJsonObject toJson() const override;
    void update(const QJsonObject &obj) override;
    TriggerType type() const override;

protected:
    void tagSocketValueChanged(TagSocket *tagSocket) override;

private:
    bool isValidTagSocketType(TagSocket *tagSocket);
    double getValueAsDouble(TagSocket *tagSocket);

    double triggerValue_ = 0.;
};

#endif // TRIGGEREVERYTIMEBELOW_H
