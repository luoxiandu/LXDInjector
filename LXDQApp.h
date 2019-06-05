#pragma once
#include <QApplication>
class LXDQApp :
	public QApplication
{
public:
	LXDQApp(int argc, char* argv[]);
	~LXDQApp();
	QString sessionkey;
	QString ver;
	QString host;
};

