#pragma once

#include <QDialog>
#include "ui_frmLogin.h"
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QSettings>

class frmLogin : public QDialog
{
	Q_OBJECT

public:
	frmLogin(QWidget *parent = Q_NULLPTR);
	~frmLogin();

private:
	Ui::frmLogin ui;
	QNetworkAccessManager *accessmanager;
	QSettings *settings;
	bool isBeggar = false;
	void login();
	void triallogin();
	void exit();
	void joinus();

signals:
	void sendSessionKey(QString skey);
	
private slots:
	void on_btnLogin_clicked();
	void on_btnJoinus_clicked();
	void on_btnQuit_clicked();
	void on_login_recieved(QNetworkReply *rep);
};
