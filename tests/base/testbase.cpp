#include "testbase.h"

#include <tagsystem/util/tagutil.h>

Tag *TestBase::createTag(
	const QString &tagName, TagType type)
{
	auto [subsystem, name] = util::tag::splitFullName(tagName);
	return TagList::sGetInstance().createTag(subsystem, name, type);
}

void TestBase::SetUp()
{
	// create initial tag, that is assumed to be created by server
	TagList::sGetInstance().createTag("system", "time", TagType::eTime);
}
