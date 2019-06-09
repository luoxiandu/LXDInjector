#pragma once

#include <QDialog>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QMessageBox>
#include "LXDQApp.h"
#include "ui_frmGameAccountService.h"

class frmGameAccountService : public QDialog
{
	Q_OBJECT

public:
	frmGameAccountService(QWidget *parent = Q_NULLPTR);
	~frmGameAccountService();

private:
	int megalodon = 0, whale = 0, greatwhite = 0, bullshark = 0, tigershark = 0, redshark = 0, level = 0;
	qreal megalodonprice = 0, whaleprice = 0, greatwhiteprice = 0, bullsharkprice = 0, tigersharkprice = 0, redsharkprice = 0, levelprice = 0, unlockprice = 0;
	bool unlock = false;
	void docalculate();
	QNetworkAccessManager accessmanager;
	Ui::frmGameAccountService ui;

private slots:
	void on_btnOrder_clicked();
	void on_btnClose_clicked();
	void on_sbMegalodon_valueChanged(int);
	void on_sbWhale_valueChanged(int);
	void on_sbGreatwhite_valueChanged(int);
	void on_sbBullshark_valueChanged(int);
	void on_sbTigershark_valueChanged(int);
	void on_sbRedshark_valueChanged(int);
	void on_sbLevel_valueChanged(int);
	void on_cbUnlock_stateChanged(int);
	void on_OrderResult_got(QNetworkReply *);
};
