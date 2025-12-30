#include "valueitem.h"
#include <tagsystem/tagsocketlist.h>
#include <tagsystem/util/tagutil.h>

ValueItem::ValueItem(TagList &tagList, const QString &triggerName, const QJsonObject &item)
    : QObject()
    , tagList_(tagList)
{
    name_ = item.value("name").toString();
    targetValue_ = item.value("targetvalue").toVariant();
    operator_ = stringToOperator(item.value("operator").toString());

    tagName_ = item.value("tagname").toString();

    auto [subsystem, name] = util::tag::splitFullName(tagName_);

    auto *tag = tagList_.findByTagName(tagName_);
    if (tag) {
        value_.reset(
            TagSocket::createTagSocket(triggerName, name_, TagSocket::typeMatchingTag(tag)));
    } else {
        // if tag does not exist tagsocket should be loaded from file at startup and exist.
        value_.reset(TagSocketList::sGetInstance().findTagSocketByName(triggerName, name_));
    }

    connect(value_.get(),
            qOverload<TagSocket *>(&TagSocket::valueChanged),
            this,
            &ValueItem::onValueChanged);
    value_->hookupTag(subsystem, name);
}

void ValueItem::onValueChanged(TagSocket *tagSocket)
{
    if (tagSocket->getType() == TagSocket::eBool) {
        bool value;
        if (tagSocket->readValue(value)) {
            currentValue_ = value;
        }
    } else if (tagSocket->getType() == TagSocket::eInt) {
        int value;
        if (tagSocket->readValue(value)) {
            currentValue_ = value;
        }
    } else if (tagSocket->getType() == TagSocket::eDouble) {
        double value;
        if (tagSocket->readValue(value)) {
            currentValue_ = value;
        }
    } else {
        return;
    }
}

bool ValueItem::evaluate()
{
    if (!operator_.has_value())
        return false;

    if (value_->getType() == TagSocket::eBool)
        return evaluateBool();
    else if (value_->getType() == TagSocket::eInt)
        return evaluateInt();
    else if (value_->getType() == TagSocket::eDouble)
        return evaluateDouble();

    // unsuported tag type always false, should not happen
    return false;
}

bool ValueItem::evaluateBool()
{
    bool target = targetValue_.toBool();
    bool value = currentValue_.toBool();

    if (operator_ == Operator::eLess) {
        return target > value;
    } else if (operator_ == Operator::eAbove) {
        return target < value;
    } else if (operator_ == Operator::eEqual) {
        return target == value;
    } else if (operator_ == Operator::eNotEqual)
        return target != value;

    Q_UNREACHABLE();
}

bool ValueItem::evaluateInt()
{
    int target = targetValue_.toInt();
    int value = currentValue_.toInt();

    if (operator_ == Operator::eLess) {
        return target > value;
    } else if (operator_ == Operator::eAbove) {
        return target < value;
    } else if (operator_ == Operator::eEqual) {
        return target == value;
    } else if (operator_ == Operator::eNotEqual)
        return target != value;

    Q_UNREACHABLE();
}

bool ValueItem::evaluateDouble()
{
    double target = targetValue_.toDouble();
    double value = currentValue_.toDouble();

    if (operator_ == Operator::eLess) {
        return target > value;
    } else if (operator_ == Operator::eAbove) {
        return target < value;
    } else if (operator_ == Operator::eEqual) {
        return target == value;
    } else if (operator_ == Operator::eNotEqual)
        return target != value;

    Q_UNREACHABLE();
}

std::optional<Operator> ValueItem::stringToOperator(const QString &str)
{
    if (str == "less")
        return Operator::eLess;
    else if (str == "above")
        return Operator::eAbove;
    else if (str == "equal")
        return Operator::eEqual;
    else if (str == "not_equal")
        return Operator::eNotEqual;

    qDebug() << "ValueItem - Unsoported operator, " << str;
    return std::nullopt;
}
