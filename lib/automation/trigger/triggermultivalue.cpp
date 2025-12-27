#include "triggermultivalue.h"

TriggerMultiValue::TriggerMultiValue(TagList &tagList, const QJsonObject &obj)
    : TriggerBase(tagList, obj)
{}

/**
 * 
 * rule<string> rule items with operator between.
 * items in the rule has to exist as items in the json array.
 * example "ruleItem1 and ruleItem2"
 * 
 * jsonarray with rule items
 * 
 * item:
 * {
 *  name<string>
 *  tagName<string>
 *  targetValuefortag<value>
 *  operator<> less/above/equal/not_equal
 * }
 */
void TriggerMultiValue::update(const QJsonObject &obj)
{
    TriggerBase::update(obj);

    //
}
