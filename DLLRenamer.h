#pragma once

#include <QObject>
#include <QFile>
#include <QFileInfo>
#include <QDateTime>
#include <QDir>
#include <QTimer>

class DLLRenamer : public QObject
{
	Q_OBJECT

public:
	DLLRenamer();
	~DLLRenamer();

private:
	QStringList paths;
	QTimer *renameTimer;

public slots:
	void dorename();
	void setpath(QString path);
};
