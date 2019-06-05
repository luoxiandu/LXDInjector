#pragma once

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QWebSocket>
#include <QProcess>
#include <QFile>
#include <QFileInfo>
#include <QMessageBox>
#include <QSettings>
#include <QList>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDir>
#include "LXDQApp.h"

class DLLHandler : public QObject
{
	Q_OBJECT

public:
	DLLHandler();
	~DLLHandler();

private:
	QString sessionkey;
	QNetworkAccessManager *accessmanager;
	bool Inject(QString path, QByteArray xpr);
	void DLLDownload(QString id);

public slots:
	void doDLLget(QString id, QString sessionkey);

private slots:
	void on_dll_downloaded(QNetworkReply *rep);
	void on_progress_reported(qint64 rec, qint64 tot);

signals:
	void statusreport(wchar_t *status);
	void statusreport(QString msg, int ms);
	void DLLfilereport(QString DLLpath);
	void finished(bool);
};
