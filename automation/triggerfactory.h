#ifndef TRIGGERFACTORY_H
#define TRIGGERFACTORY_H

#include <map>
#include <string>
#include <iostream>
#include <QJsonObject>


class TriggerBase;
class TagList;

template<typename Type>
TriggerBase* createType(TagList &tagList, const QJsonObject &obj) { return new Type(tagList, obj); }

class TriggerFactory
{
public:
    TriggerFactory(){}

    TriggerBase* createTrigger(const std::string &key, TagList &tagList, const QJsonObject &obj)
    {
        if(constructors_.find(key) == constructors_.end())
            return nullptr;
        Creator createor = constructors_[key];
        return createor(tagList, obj);
    }

    template<typename Type>
    void addFactory(const std::string &aCreateKey);


protected:
    typedef TriggerBase* (*Creator)(TagList &, const QJsonObject&);

    std::map<std::string, Creator> constructors_;
};


template<typename Type>
void TriggerFactory::addFactory(const std::string &aCreateKey)
{
    Creator creator = &createType<Type>;
    constructors_.insert(std::make_pair(aCreateKey, creator));
}

#endif // TRIGGERFACTORY_H
