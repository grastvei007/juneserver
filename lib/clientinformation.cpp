#include "clientinformation.h"

#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

ClientInformation::ClientInformation(QString aInfo, QObject *parent) : QObject(parent),
    mHasError(false)
{
	auto jsonObject = QJsonDocument::fromJson(aInfo.toLatin1()).object();

	if(jsonObject.contains("name"))
	{
		mName = jsonObject.value("name").toString();
	}
	if(jsonObject.contains("ip"))
	{
		mIp = jsonObject.value("ip").toString();
	}
	if(jsonObject.contains("tag_set"))
	{
		const auto &array = jsonObject.value("tag_set").toArray();
		for(const auto &tag : array)
		{
			tagSet_.push_back(tag.toString());
		}
	}



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
}


QString ClientInformation::getName() const
{
    return mName;
}


QString ClientInformation::getIp() const
{
    return mIp;
}

QStringList ClientInformation::getTagSet() const
{
	return tagSet_;
}

bool ClientInformation::hasTagSet() const
{
	return !tagSet_.empty();
}


bool ClientInformation::hasError() const
{
    return mHasError;
}


QString ClientInformation::errorStr() const
{
    return mErrorString;
}
