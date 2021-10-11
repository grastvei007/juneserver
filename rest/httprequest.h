#ifndef HTTPREQUEST_H
#define HTTPREQUEST_H

#include <QByteArray>
#include <QString>

class HttpRequest
{
public:
    enum RequestType
    {
        eNone,
        ePost,
        eGet
    };
    HttpRequest(const QByteArray &data);


    RequestType requestType() const;
    QString command() const;
private:
    RequestType requestType_ = eNone;
    QString cmd_;
};

#endif // HTTPREQUEST_H
