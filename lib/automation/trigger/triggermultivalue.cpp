#include "triggermultivalue.h"

#include <QJsonArray>

#include "valueitem.h"

TriggerMultiValue::TriggerMultiValue(TagList &tagList, const QJsonObject &obj)
    : TriggerBase(tagList, obj)
{
    TriggerMultiValue::update(obj);
}

QJsonObject TriggerMultiValue::toJson() const
{
    auto json = TriggerBase::toJson();
    //

    return json;
}

/**
 * 
 * rule<string> rule items with operator between.
 * items in the rule has to exist as items in the json array.
 * example "ruleItem1 and ruleItem2"
 * 
 * rule operators supported and|or
 * 
 * jsonarray with rule items
 * 
 * item:
 * {
 *  name<string>
 *  tagName<string>
 *  targetValue<value>
 *  operator<> less/above/equal/not_equal
 * }
 */
void TriggerMultiValue::update(const QJsonObject &obj)
{
    TriggerBase::update(obj);

	if (obj.contains("duration"))
	{
		duration_ = obj.value("duration").toInt();
	}
	if (obj.contains("triggeronceaday"))
	{
		triggerOnceADay_ = obj.value("triggeronceaday").toBool();
	}

	if (obj.contains("rule"))
	{
		rule_ = obj.value("rule").toString();
	}

	if (obj.contains("items"))
	{
		const QJsonArray itemList = obj.value("items").toArray();
		valueItems_.clear();

		for (const auto ref : itemList)
		{
			const auto &obj = ref.toObject();
			auto name = obj.value("name").toString();
			valueItems_.emplace(name, std::make_unique<ValueItem>(tagList(), triggerName(), obj));
		}
	}
}

TriggerType TriggerMultiValue::type() const
{
    return TriggerType::TriggerMultiValue;
}

void TriggerMultiValue::tagSocketValueChanged(TagSocket *tagSocket)
{
	auto isOdd = [](unsigned int n) { return !(n % 2); };

	auto hasOnlyOneOperator = [](const QStringList &words) {
		auto op = words[1];
		for (int i = 1; i < words.size(); i += 2)
		{
			if (words[i] != op)
				return false;
		}
		return true;
	};

	auto validateItems = [this](const QStringList &words) {
		bool ok = true;
		for (int i = 0; i < words.size(); i += 2)
		{
			if (!valueItems_.contains(words[i]))
			{
				qDebug() << "Invalid word in rule: " << words[i];
				ok = false;
			}
		}
		return ok;
	};

	if (rule_.startsWith("("))
	{
		//TODO: parse more advanced logic
	}
	else
	{
		// singel item
		if (!rule_.contains(QChar::Space))
		{
			if (!valueItems_.contains(rule_))
				return;

			if (valueItems_[rule_]->evaluate())
			{
				if (!isActive())
				{
					setActive();
					triggerTime_ = QDateTime::currentMSecsSinceEpoch();
				}
			}
			else if (isActive())
			{
				qint64 now = QDateTime::currentMSecsSinceEpoch();
				if (now > triggerTime_ + duration_)
				{
					setDeactive();
				}
			}
		}
		else if (auto words = rule_.split(QChar::Space);
				 isOdd(words.size())) // is series of items with operators
		{
			if (!hasOnlyOneOperator(words))
			{
				qDebug() << "Not supported rule with different operators";
				return;
			}
			if (!validateItems(words))
				return;

			if (auto op = words[1]; op == "and")
			{
				if (isAllItemsTrue(words))
				{
					if (!isActive())
					{
						setActive();
						triggerTime_ = QDateTime::currentMSecsSinceEpoch();
					}
				}
				else if (isActive())
				{
					qint64 now = QDateTime::currentMSecsSinceEpoch();
					if (now > triggerTime_ + duration_)
					{
						setDeactive();
					}
				}
			}
			else if (op == "or")
			{
				if (isOneItemTrue(words))
				{
					if (!isActive())
					{
						setActive();
						triggerTime_ = QDateTime::currentMSecsSinceEpoch();
					}
				}
				else if (isActive())
				{
					qint64 now = QDateTime::currentMSecsSinceEpoch();
					if (now > triggerTime_ + duration_)
					{
						setDeactive();
					}
				}
			}
			else
			{
				qDebug() << "Unsuported operator: " << op;
			}
		}
		else
		{
			qDebug() << "Unsuported rule: " << rule_;
		}
	}
}

bool TriggerMultiValue::isAllItemsTrue(const QStringList &words)
{
	for (int i = 0; i < words.size(); i += 2)
	{
		if (!valueItems_[words[i]]->evaluate())
			return false;
	}

	return true;
}

bool TriggerMultiValue::isOneItemTrue(const QStringList &words)
{
	for (int i = 0; i < words.size(); i += 2)
	{
		if (valueItems_[words[i]]->evaluate())
			return true;
	}
	return false;
}
