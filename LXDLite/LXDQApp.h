#pragma once
#include <QApplication>
#include <QJsonObject>
#include "ThemidaSDK.h"
#include <windows.h>
class LXDQApp :
	public QApplication
{
public:
	LXDQApp(int argc, char* argv[]);
	~LXDQApp();
	QString passkey;
	QString HWID;
	QString remaining;
	QString ver;
	QString host;
	QJsonObject prices;
};

