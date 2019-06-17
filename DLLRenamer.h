#pragma once

#include <QObject>
#include <QFile>
#include <QFileInfo>
#include <QDateTime>
#include <QDir>
#include <QTimer>
#include <QDebug>
#include <QSharedMemory>
#include "LXDQApp.h"

class DLLRenamer : public QObject
{
	Q_OBJECT

public:
	DLLRenamer();
	~DLLRenamer();

private:
	QStringList paths;
	QTimer *renameTimer;
	QSharedMemory mem;
	QStringList *tomem;

public slots:
	void dorename();
	void setpath(QString path);
};
