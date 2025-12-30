#ifndef TRIGGERTWOVALUES_H
#define TRIGGERTWOVALUES_H

#include <tagsystem/taglist.h>

#include "automation/trigger.h"

class TriggerTwoValues : public TriggerBase
{
public:
    TriggerTwoValues(TagList &tagList, const QJsonObject &obj, QObject *parent = nullptr);

    void update(const QJsonObject &obj) final;
    QJsonObject toJson() const final;

    TriggerType type() const final;

protected:
    void tagSocketValueChanged(TagSocket *tagSocket) override;

private:
    bool isValidTagSocketType(TagSocket *tagSocket);
    double getValueAsDouble(TagSocket *tagSocket);

    bool triggerOnBelowValue_ = false;
    bool triggerOnAboveValue_ = false;
    bool triggerOffBelowValue_ = false;
    bool triggerOffAboveValue_ = false;

    double valueTriggerOn_ = 0.0;
    double valueTriggerOff_ = 0.0;
};

#endif // TRIGGERTWOVALUES_H
