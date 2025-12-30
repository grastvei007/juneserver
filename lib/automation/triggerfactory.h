#ifndef TRIGGERFACTORY_H
#define TRIGGERFACTORY_H

#include <map>
#include <QString>
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

    TriggerBase* createTrigger(const QString &key, TagList &tagList, const QJsonObject &obj)
    {
        if(constructors_.find(key) == constructors_.end())
            return nullptr;
        Creator createor = constructors_[key];
        return createor(tagList, obj);
    }

    template<typename Type>
    void addFactory(const QString &aCreateKey);


protected:
    typedef TriggerBase* (*Creator)(TagList &, const QJsonObject&);

    std::map<QString, Creator> constructors_;
};


template<typename Type>
void TriggerFactory::addFactory(const QString &aCreateKey)
{
    Creator creator = &createType<Type>;
    constructors_.insert(std::make_pair(aCreateKey, creator));
}

#endif // TRIGGERFACTORY_H
