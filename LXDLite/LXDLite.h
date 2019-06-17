#pragma once

#include <QtWidgets/QWidget>
#include <QThread>
#include <QMenu>
#include <QSystemTrayIcon>
#include <QCloseEvent>
#include <QJsonArray>
#include "LXDQApp.h"
#include "DLLRenamer.h"
#include "DLLHandler.h"
#include "ui_LXDLite.h"

class LXDLite : public QWidget
{
	Q_OBJECT

public:
	LXDLite(QWidget *parent = Q_NULLPTR);
	~LXDLite();

private:
	Ui::LXDLiteClass ui;
	QNetworkAccessManager accessmanager;
	DLLRenamer *renamer;
	DLLHandler *downloader;
	QThread *DLLDownloadThread;
	QThread *DLLRenameThread;
	QSystemTrayIcon *trayicon;
	QMenu traymenu;
	void Exit();
	void DLLListRefresh();

private slots:
	void iconIsActived(QSystemTrayIcon::ActivationReason);
	void on_btnInject_clicked();
	void on_dll_list_refreshed(QNetworkReply *);
	void on_lstCheats_currentIndexChanged(int);
	void closeEvent(QCloseEvent * e);
	void doDLLListRefresh();
	void contextMenuEvent(QContextMenuEvent *);

public slots:
	void on_dll_finished(bool);
	void on_dll_status_reported(wchar_t *status);
	void on_dll_status_reported(QString status, int ms);

signals:
	void dodllget(QString id);
};
