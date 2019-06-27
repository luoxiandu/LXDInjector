#pragma once
#include <QApplication>
#include <QJsonObject>
#include <QSslConfiguration>
#include <QLocalSocket>
#include "ThemidaSDK.h"
class LXDQApp :
	public QApplication
{
public:
	LXDQApp(int argc, char* argv[]);
	~LXDQApp();
	QString sessionkey;
	QString ver;
	QString host;
	QJsonObject prices;
	QSslConfiguration sslConfig;
	QLocalSocket GuardSocket;
};

