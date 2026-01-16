#ifndef TESTBASE_H
#define TESTBASE_H

#include <gtest/gtest.h>
#include <tagsystem/taglist.h>

class TestBase : public ::testing::Test
{
  public:
	TestBase() = default;

	Tag *createTag(const QString &tagName, TagType type);

  protected:
	void SetUp() override;

	void TearDown() override { TagList::sGetInstance().clear(); }
};

#endif // TESTBASE_H
