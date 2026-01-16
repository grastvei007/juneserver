#ifndef TESTTRIGGERBASE_H
#define TESTTRIGGERBASE_H

#include <QString>
#include "testbase.h"
#include <tagsystem/taglist.h>

class TestTriggerBase : public TestBase
{
  public:
	TestTriggerBase();

	QJsonObject createItemJson(const QString &name,
							   const QString &tagName,
							   QVariant value,
							   const QString &op,
							   TagType type);

  protected:
};

#endif // TESTTRIGGERBASE_H
