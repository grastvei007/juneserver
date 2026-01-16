#include <gtest/gtest.h>

#include <QJsonObject>

#include <tagsystem/tag.h>
#include <tagsystem/taglist.h>

class TestTriggerMultiValue : public ::testing::Test
{
  protected:
	void SetUp() override {}

	void TearDown() override { TagList::sGetInstance().clear(); }
};

TEST_F(
	TestTriggerMultiValue, eval)
{
	//
}
