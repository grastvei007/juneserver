#include <gtest/gtest.h>

#include <QJsonObject>

#include <tagsystem/tag.h>
#include <tagsystem/taglist.h>
#include <tagsystem/util/tagutil.h>

#include <automation/trigger/valueitem.h>

class TestValueItem : public ::testing::Test
{
protected:
    void SetUp() override {}

    void TearDown() override { TagList::sGetInstance().clear(); }

    QJsonObject createItemJson(const QString &name,
                               const QString &tagName,
                               QVariant value,
                               const QString &op,
                               TagType type)
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

    Tag *createTag(const QString &tagName, TagType type)
    {
        auto [subsystem, name] = util::tag::splitFullName(tagName);
        return TagList::sGetInstance().createTag(subsystem, name, type);
    }

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
