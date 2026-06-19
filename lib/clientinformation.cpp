#include "clientinformation.h"

#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

ClientInformation::ClientInformation(QString info, QObject *parent) : QObject(parent)
{
	auto jsonObject = QJsonDocument::fromJson(info.toLatin1()).object();

	if(jsonObject.contains("name"))
	{
		name_ = jsonObject.value("name").toString();
	}
	if(jsonObject.contains("ip"))
	{
		ip_ = jsonObject.value("ip").toString();
	}
	if(jsonObject.contains("tag_set"))
	{
		const auto &array = jsonObject.value("tag_set").toArray();
		for(const auto &tag : array)
		{
			tagSet_.push_back(tag.toString());
		}
	}



	if(name_.isEmpty())
	{
		errorString_.append("ClientInformation, Name not set.\n");
		hasError_ = true;
	}
	else if(ip_.isEmpty())
	{
		errorString_.append("ClientInformation, Ip not set.\n");
		hasError_ = true;
	}
}


QString ClientInformation::getName() const
{
	return name_;
}


QString ClientInformation::getIp() const
{
	return ip_;
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
	return hasError_;
}


QString ClientInformation::errorStr() const
{
	return errorString_;
}
