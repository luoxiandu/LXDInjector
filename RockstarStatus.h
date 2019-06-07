#pragma once

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QMessageBox>
#include <QIcon>

class RockstarStatus : public QObject
{
	Q_OBJECT

public:
	RockstarStatus();
	~RockstarStatus();

private:
	QNetworkAccessManager accessmanager;

public slots:
	void doget();

private slots:
	void on_get_finished(QNetworkReply *);

signals:
	void reporterr(QString);
	void finishget(QString);
};
