#pragma once

#include <QDialog>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include "LXDQApp.h"
#include "ui_Deposit.h"

class Deposit : public QDialog
{
	Q_OBJECT

public:
	Deposit(QWidget *parent = Q_NULLPTR);
	~Deposit();

private:
	QNetworkAccessManager accessmanager;
	Ui::Deposit ui;

private slots:
	void on_btnGetCode_clicked();
	void on_btnClose_clicked();
	void on_QRcode_got(QNetworkReply *);
};
