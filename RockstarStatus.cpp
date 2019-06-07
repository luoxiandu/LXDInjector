#include "RockstarStatus.h"

RockstarStatus::RockstarStatus()
	: QObject()
{
}

RockstarStatus::~RockstarStatus()
{
}

void RockstarStatus::doget()
{
	QNetworkReply *rep = accessmanager.get(QNetworkRequest(QUrl("https://support.rockstargames.com/services/status.json?tz=Asia/Shanghai")));
	if (rep->error() == QNetworkReply::NoError)
	{
		connect(&accessmanager, SIGNAL(finished(QNetworkReply*)), this, SLOT(on_get_finished(QNetworkReply*)));
	}
	else
	{
		emit reporterr(QString::fromWCharArray(L"获取失败！"));
	}
	return;
}

void RockstarStatus::on_get_finished(QNetworkReply *rep)
{
	QString result;
	QString respstr = rep->readAll();
	rep->deleteLater();
	QJsonDocument respjson = QJsonDocument::fromJson(respstr.toUtf8().data());
	if (respjson.isNull() || respjson.isEmpty())
	{
		emit reporterr(QString::fromWCharArray(L"获取失败！"));
	}
	else
	{
		result.append(QString::fromWCharArray(L"根据Rockstar官方通报，目前GTA Online各平台服务器状态如下：\n"));
		QJsonObject obj = respjson.object();
		QJsonArray statuses = obj.value("statuses").toArray();
		QJsonArray::iterator i = statuses.begin();
		while (i != statuses.end())
		{
			if ((*i).toObject().value("tag").toString() == "gtao")
			{
				QJsonArray platforms = (*i).toObject().value("services_platforms").toArray();
				QJsonArray::iterator j = platforms.begin();
				while (j != platforms.end())
				{
					QJsonObject platform = (*j).toObject();
					int platformstatus = platform.value("service_status_id").toInt();
					result.append(platform.value("name").toString());
					result.append(QString::fromWCharArray(L"平台："));
					result.append(platformstatus == 1 ? QString::fromWCharArray(L"服务正常\n") : QString::fromWCharArray(L"服务不正常，内部错误代码为：%1\n").arg(platformstatus));
					j++;
				}
			}
			else if ((*i).toObject().value("tag").toString() == "sc")
			{
				int scstatus = (*i).toObject().value("services_platforms").toArray()[0].toObject().value("service_status_id").toInt();
				result.append(QString::fromWCharArray(L"Rockstar Social Club 服务状态："));
				result.append(scstatus == 1 ? QString::fromWCharArray(L"服务正常") : QString::fromWCharArray(L"服务不正常，内部错误代码为：%1").arg(scstatus));
			}
			i++;
		}
	}
	disconnect(&accessmanager, SIGNAL(finished(QNetworkReply*)), this, SLOT(on_get_finished(QNetworkReply*)));
	emit finishget(result);
	return;
}
