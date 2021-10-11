#ifndef REQUESTLISTENER_H
#define REQUESTLISTENER_H

#include <QString>

#include "respons.h"

class RequestListener
{
public:
    RequestListener();
    RequestListener(const QString &request, const Respons &respons);

    virtual bool isRequest(const QString &currentRequest) = 0;
    virtual Respons executeRequest(QString command) = 0;

protected:
    const QString& request() const;
    const Respons& respons() const;
private:
    QString request_;
    Respons respons_;

};

#endif // REQUESTLISTENER_H
