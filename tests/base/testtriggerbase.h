#ifndef TESTTRIGGERBASE_H
#define TESTTRIGGERBASE_H

#include <gtest/gtest.h>
#include <tagsystem/taglist.h>

class TestTriggerBase : public ::testing::Test
{
  public:
	TestTriggerBase();

	QJsonObject createItemJson(const QString &name,
							   const QString &tagName,
							   QVariant value,
							   const QString &op,
							   TagType type);

  protected:
	void SetUp() override {}

	void TearDown() override { TagList::sGetInstance().clear(); }
};

#endif // TESTTRIGGERBASE_H
