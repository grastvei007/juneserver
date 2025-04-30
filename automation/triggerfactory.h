#ifndef TRIGGERFACTORY_H
#define TRIGGERFACTORY_H

#include <map>
#include <string>
#include <iostream>
#include <QJsonObject>


class TriggerBase;

template<typename Type>
TriggerBase* createType(const QJsonObject &obj) { return new Type(obj); }

class TriggerFactory
{
public:
    TriggerFactory(){}

    TriggerBase* createTrigger(const std::string &key, const QJsonObject &obj)
    {
        if(constructors_.find(key) == constructors_.end())
            return nullptr;
        Creator createor = constructors_[key];
        return createor(obj);
    }

    template<typename Type>
    void addFactory(const std::string &aCreateKey);


protected:
    typedef TriggerBase* (*Creator)(const QJsonObject&);

    std::map<std::string, Creator> constructors_;
};


template<typename Type>
void TriggerFactory::addFactory(const std::string &aCreateKey)
{
    Creator creator = &createType<Type>;
    constructors_.insert(std::make_pair(aCreateKey, creator));
}

#endif // TRIGGERFACTORY_H
