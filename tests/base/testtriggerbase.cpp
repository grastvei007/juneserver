#include "testtriggerbase.h"

TestTriggerBase::TestTriggerBase() {}

QJsonObject TestTriggerBase::createItemJson(
	const QString &name, const QString &tagName, QVariant value, const QString &op, TagType type)
{
	QJsonObject obj;
	obj.insert("name", name);
	obj.insert("tagname", tagName);
	obj.insert("operator", op);
	if (type == TagType::eBool)
		obj.insert("targetvalue", value.toBool());
	else if (type == TagType::eInt)
		obj.insert("targetvalue", value.toInt());
	else if (type == TagType::eDouble)
		obj.insert("targetvalue", value.toDouble());

	return obj;
}

QJsonObject TestTriggerBase::createTriggerBase(
	const QString &subsystem,
	const QString &name,
	const QString &triggerName,
	TriggerType triggerType,
	bool isEnabled)
{
	QJsonObject obj;

	obj.insert("subsystem", subsystem);
	obj.insert("name", name);
	obj.insert("triggername", triggerName);
	obj.insert("type", triggerTypeToString(triggerType));
	obj.insert("enable", isEnabled);

	return obj;
}

void TestTriggerBase::extendTriggerToMultiValue(
	QJsonObject &triggerBase, int duration, const QString &rule, QJsonArray &items)
{
	triggerBase.insert("duration", duration);
	triggerBase.insert("rule", rule);
	triggerBase.insert("items", items);
}
