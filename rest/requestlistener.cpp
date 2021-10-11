#include "requestlistener.h"

RequestListener::RequestListener()
{

}

RequestListener::RequestListener(const QString &request, const Respons &respons) :
    request_(request),
    respons_(respons)
{

}

const QString &RequestListener::request() const
{
    return  request_;
}

const Respons &RequestListener::respons() const
{
    return respons_;
}
