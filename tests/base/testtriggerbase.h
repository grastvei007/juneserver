#ifndef TESTTRIGGERBASE_H
#define TESTTRIGGERBASE_H

#include <QString>
#include "testbase.h"
#include <tagsystem/taglist.h>

#include <automation/trigger.h>

class TestTriggerBase : public TestBase
{
  public:
	TestTriggerBase();

	QJsonObject createItemJson(const QString &name,
							   const QString &tagName,
							   QVariant value,
							   const QString &op,
							   TagType type);

	QJsonObject createTriggerBase(const QString &subsystem,
								  const QString &name,
								  const QString &triggerName,
								  TriggerType triggerType,
								  bool isEnabled);

	// add values to triggerBase
	void extendTriggerToMultiValue(QJsonObject &triggerBase,
								   int duration,
								   const QString &rule,
								   QJsonArray &items);

  protected:
};

#endif // TESTTRIGGERBASE_H
