#pragma once

#include <QtWidgets/QMainWindow>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QWebSocket>
#include <QProcess>
#include <QSettings>
#include <QList>
#include <QSystemTrayIcon>
#include <QThread>
#include <QLocalSocket>
#include "ui_LXDInjector.h"
#include "LXDQApp.h"
#include "DLLHandler.h"
#include "DLLRenamer.h"
#include "Deposit.h"
#include "frmGameAccountService.h"
#include "frmAdvertisement.h"
#include "RockstarStatus.h"

class LXDInjector : public QMainWindow
{
	Q_OBJECT

public:
	LXDInjector(QWidget *parent = Q_NULLPTR);
	~LXDInjector();
	void setStatus(wchar_t *status);

private:
	int developercount = 0;
	bool isBeggar = true;
	bool loginoutdated = false;
	bool isFirstRun = true;
	int networkerrorcount = 0;
	Ui::LXDInjectorClass ui;
	QNetworkAccessManager *accessmanager;
	QString sessionkey;
	QSettings *settings;
	QTimer *refreshTimer;
	QWebSocket *chksocket;
	DLLRenamer *renamer;
	DLLHandler *downloader;
	QThread *DLLDownloadThread;
	QThread *DLLRenameThread;
	QThread *RockstarStatusThread;
	RockstarStatus *RockstarStatusworker;
	QSystemTrayIcon *trayicon;
	QMenu *traymenu;
	QLocalSocket GuardSocket;
	frmAdvertisement adv;
	// bool Inject(QString path, QByteArray xpr);
	// void DLLDownload(QString id);
	void DLLListRefresh();
	void AccountInfoRefresh();
	

private slots:
	void chklogin();
	void wsconnected();
	void wsdisconnected();
	void wsrecieved(QString);
	void on_btnInject_clicked();
	void on_btnTranslate_clicked();
	void on_actionExit_triggered();
	void on_actionAbout_triggered();
	void on_actionRefresh_triggered();
	void on_actionHelp_triggered();
	void on_actiongetRockstarStatus_triggered();
	void on_actionRefreshAccount_triggered();
	void on_actionDeposit_triggered();
	void on_actionGameAccountService_triggered();
	void on_lstCheats_currentIndexChanged(int);
	void on_progress_reported(qint64, qint64);
	void on_dll_list_refreshed(QNetworkReply *rep);
	void on_account_info_refreshed(QNetworkReply *rep);
	void iconIsActived(QSystemTrayIcon::ActivationReason);
	void closeEvent(QCloseEvent * e);
	void contextMenuEvent(QContextMenuEvent *e);
	void on_guard_died();

public slots:
	void on_dll_finished(bool);
	void on_dll_status_reported(wchar_t *status);
	void on_dll_status_reported(QString status, int ms);
	void on_RockstarStatus_got(QString result);
	void showError(QString errstr);

signals:
	void dodllget(QString id, QString sessionkey);
	void doRockstarStatusget();
};
