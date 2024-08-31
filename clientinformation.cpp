#include "clientinformation.h"

#include <QDebug>
#include <QXmlStreamReader>


ClientInformation::ClientInformation(QString aInfo, QObject *parent) : QObject(parent),
    mHasError(false)
{
    QXmlStreamReader stream(aInfo);
    while(!stream.atEnd() && !stream.hasError())
    {
       QXmlStreamReader::TokenType token = stream.readNext();
       if(token == QXmlStreamReader::StartDocument)
           continue;
       if(token == QXmlStreamReader::StartElement)
       {
           if(stream.name() == QString("client"))
               continue;

           if(stream.name() == QString("name"))
           {
               mName = stream.readElementText();
               qDebug() << mName;
           }
           if(stream.name() == QString("ip"))
           {
               mIp = stream.readElementText();
               qDebug() << mIp;
           }
       }

    }
   /* if(stream.hasError())
    {
        mErrorString = stream.errorString();
        mHasError = true;
    }
    else
    {*/
        if(mName.isEmpty())
        {
            mErrorString.append("ClientInformation, Name not set.\n");
            mHasError = true;
        }
        else if(mIp.isEmpty())
        {
            mErrorString.append("ClientInformation, Ip not set.\n");
            mHasError = true;
        }
   // }
}


QString ClientInformation::getName() const
{
    return mName;
}


QString ClientInformation::getIp() const
{
    return mIp;
}


bool ClientInformation::hasError() const
{
    return mHasError;
}


QString ClientInformation::errorStr() const
{
    return mErrorString;
}
