#pragma once

#include <QObject>
#include <QTimer>
#include "LXDQApp.h"

class SafeGuard : public QObject
{
	Q_OBJECT

private:
	QTimer *timer;

public:
	SafeGuard();
	~SafeGuard();

private slots:
	void doguard();
};
