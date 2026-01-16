#include "base/testtriggerbase.h"

#include <QJsonObject>

#include <tagsystem/tag.h>
#include <tagsystem/taglist.h>
#include <tagsystem/util/tagutil.h>

#include <automation/trigger/valueitem.h>

class TestValueItem : public TestTriggerBase
{
protected:
    ValueItem creteValueItem(const QJsonObject &obj)
    {
        return ValueItem(TagList::sGetInstance(), "testName", obj);
    }
};

TEST_F(TestValueItem, evaluate_bool_valueitem)
{
    auto type = TagType::eBool;
    auto tagName = QString("test.tag");
    Tag *tag = createTag(tagName, type);
    tag->setValue(false);

    auto json = createItemJson("test", tagName, true, "equal", type);
    auto valueItem = creteValueItem(json);

    EXPECT_FALSE(valueItem.evaluate());

    tag->setValue(true);
    EXPECT_TRUE(valueItem.evaluate());
}

TEST_F(TestValueItem, evaluate_int_operatorLess_valueitem)
{
    auto type = TagType::eInt;
    auto tagName = QString("test.tag");
    Tag *tag = createTag(tagName, type);
    tag->setValue(10);

    auto json = createItemJson("test", tagName, 100, "less", type);
    auto valueItem = creteValueItem(json);

    EXPECT_TRUE(valueItem.evaluate());

    tag->setValue(105);
    EXPECT_FALSE(valueItem.evaluate());
}

TEST_F(TestValueItem, evaluate_int_operatorAbove_valueitem)
{
    auto type = TagType::eInt;
    auto tagName = QString("test.tag");
    Tag *tag = createTag(tagName, type);
    tag->setValue(10);

    auto json = createItemJson("test", tagName, 100, "above", type);
    auto valueItem = creteValueItem(json);

    EXPECT_FALSE(valueItem.evaluate());

    tag->setValue(105);
    EXPECT_TRUE(valueItem.evaluate());
}

TEST_F(TestValueItem, evaluate_double_operatorLess_valueitem)
{
    auto type = TagType::eDouble;
    auto tagName = QString("test.tag");
    Tag *tag = createTag(tagName, type);
    tag->setValue(10.0);

    auto json = createItemJson("test", tagName, 100.0, "less", type);
    auto valueItem = creteValueItem(json);

    EXPECT_TRUE(valueItem.evaluate());

    tag->setValue(105.0);
    EXPECT_FALSE(valueItem.evaluate());
}

TEST_F(TestValueItem, evaluate_double_operatorAbove_valueitem)
{
    auto type = TagType::eDouble;
    auto tagName = QString("test.tag");
    Tag *tag = createTag(tagName, type);
    tag->setValue(10.0);

    auto json = createItemJson("test", tagName, 100.0, "above", type);
    auto valueItem = creteValueItem(json);

    EXPECT_FALSE(valueItem.evaluate());

    tag->setValue(105.0);
    EXPECT_TRUE(valueItem.evaluate());
}
