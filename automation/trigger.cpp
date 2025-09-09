#include "trigger.h"
#include <tagsystem/taglist.h>
#include <tagsystem/tagsocketlist.h>

TriggerBase::TriggerBase(TagList &tagList, const QJsonObject &obj, QObject *parent) :
    tagList_(tagList),
    QObject{parent}
{
    subsystem_ = obj.value("subsystem").toString();
    name_ = obj.value("name").toString();
    triggerName_ = obj.value("triggername").toString();

    watchTag_ = tagList_.findByTagName(subsystem_, name_);

    // if the tag does not exist, will most likely happen when server is started, and it is a tag
    // created by another module. Then the tagsocket should be loaded at startup.
    // create the tag if it does not exist.
    watchTagSocket_ = TagSocketList::sGetInstance().findTagSocketByName("trigger", triggerName_);
    if(!watchTag_ && watchTagSocket_)
    {
        watchTag_ = tagList.createTag(subsystem_, name_, Tag::typeMatchTagSocket(watchTagSocket_));
    }

    if(watchTag())
    {
        if(!watchTagSocket_)
            watchTagSocket_ = TagSocket::createTagSocket("trigger", triggerName(), TagSocket::typeMatchingTag(watchTag()));
        watchTagSocket_->hookupTag(watchTag());

        connect(watchTagSocket_, qOverload<TagSocket*>(&TagSocket::valueChanged), this, &TriggerBase::onTagSocketValueChanged);

        triggerTag_ = tagList_.createTag("trigger", triggerName_, Tag::eBool, isActive_, "Target trigger");
        if (triggerTag_->getBoolValue() != isActive_)
            triggerTag_->setValue(isActive_);
    }
}

bool TriggerBase::isActive() const
{
    return isActive_;
}

bool TriggerBase::hasWatchTag() const
{
    return watchTag_ != nullptr;
}

bool TriggerBase::isEnabled() const
{
    return isEnabled_;
}

bool TriggerBase::shouldSave() const
{
    return true;
}

Tag *TriggerBase::watchTag() const
{
    return watchTag_;
}

const QString &TriggerBase::subsystem() const
{
    return subsystem_;
}

const QString &TriggerBase::triggerName() const
{
    return triggerName_;
}

QJsonObject TriggerBase::toJson() const
{
    QJsonObject json;

    json.insert("subsystem", subsystem_);
    json.insert("name", name_);
    json.insert("triggername", triggerName_);
    json.insert("type", triggerTypeToString(type()));
    json.insert("enable", isEnabled_);

    return json;
}

void TriggerBase::update(const QJsonObject &obj)
{
    if (obj.contains("enable"))
    {
        isEnabled_ = obj.value("enable").toBool();
    }

}

void TriggerBase::setActive()
{
    if(triggerTag_)
    {
        isActive_ = true;
        triggerTag_->setValue(isActive_);
    }

}

void TriggerBase::setDeactive()
{
    if(triggerTag_)
    {
        isActive_ = false;
        triggerTag_->setValue(isActive_);
    }
}

bool TriggerBase::validateWatchTacksoket(TagSocket::Type type) const
{
    return watchTagSocket_->getType() == type;
}

void TriggerBase::onTagSocketValueChanged(TagSocket *tagSocket)
{
    if(isEnabled())
    {
        tagSocketValueChanged(tagSocket);
    }
}

QString TriggerBase::triggerTypeToString(TriggerType type) const
{
    switch (type) {
    case TriggerType::TriggerEveryTimeAbove:
        return "triggerAbove";
    case TriggerType::TriggerEveryTimeBelow:
        return "trigggerBelow";
    case TriggerType::TriggerOnTime:
        return "triggerOnTime";
    case TriggerType::ScheduleOnDuration:
        return "scheduleOnDuration";
    case TriggerType::TriggerTwoValues:
        return "triggertwovalues";
    default:
        break;
    }
    return {};
}
