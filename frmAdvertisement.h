#pragma once

#include <QDialog>
#include <QWebEngineView>
#include <QWebEngineProfile>
#include <QDesktopServices>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include "LXDQApp.h"
#include "ui_frmAdvertisement.h"

class frmAdvertisement : public QDialog
{
	Q_OBJECT

public:
	frmAdvertisement(QWidget *parent = Q_NULLPTR);
	~frmAdvertisement();

private:
	QJsonObject advs;
	QJsonObject::iterator i;
	QNetworkAccessManager accessmanager;
	Ui::frmAdvertisement ui;
	void showcurrent();

private slots:
	void on_btnClose_clicked();
	void on_btnPrevious_clicked();
	void on_btnNext_clicked();
	void on_data_recieved(QNetworkReply *);
	void on_urlChanged(const QUrl &);
	void on_page_load(bool);
};
