#include "httprequest.h"

#include <QStringList>
#include <QDebug>

HttpRequest::HttpRequest(const QByteArray &data)
{
    QStringList lines =  QString(data).split(QLatin1Char( '\n'));
    for(auto line : lines)
    {
        qDebug() << line;
    }
    if(!lines.empty())
    {
        QStringList parts = lines.at(0).split(" ");
        if(parts.at(0) == "GET")
            requestType_ = eGet;
        else if(parts.at(0) == "POST")
            requestType_ = ePost;

        cmd_ = parts.at(1);
    }
}

HttpRequest::RequestType HttpRequest::requestType() const
{
    return requestType_;
}

QString HttpRequest::command() const
{
    return cmd_;
}
