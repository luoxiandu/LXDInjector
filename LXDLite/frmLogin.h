#pragma once

#include <QDialog>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QSettings>
#include <QMessageBox>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include "ui_frmLogin.h"
#include "LXDQApp.h"

class frmLogin : public QDialog
{
	Q_OBJECT

public:
	frmLogin(QWidget *parent = Q_NULLPTR);
	~frmLogin();

private:
	QNetworkAccessManager accessmanager;
	Ui::frmLogin ui;

private slots:
	void on_btnLogin_clicked();
	void on_btnExit_clicked();
	void on_login_recieved(QNetworkReply *);
};
