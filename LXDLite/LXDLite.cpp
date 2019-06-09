#include "LXDLite.h"

bool CheckAppRunningStatus(const QString &appName)
{
	VM_START
#ifdef Q_OS_WIN
	QProcess* process = new QProcess;
	process->start("tasklist", QStringList() << "/FI" << "imagename eq " + appName);
	process->waitForFinished();
	QString outputStr = QString::fromLocal8Bit(process->readAllStandardOutput());
	if (outputStr.contains(appName)) {
		return true;
	}
	else {
		return false;
	}
#endif
	VM_END
}

LXDLite::LXDLite(QWidget *parent)
	: QWidget(parent)
{
	VM_START
	QIcon ico(":/LXDLite/LXDLite.ico");
	setWindowFlag(Qt::CustomizeWindowHint, true);
	setWindowFlag(Qt::WindowMinMaxButtonsHint, false);
	ui.setupUi(this);
	setWindowTitle(QString::fromWCharArray(L"洛仙都纯享版 卡密剩余有效期：") + ((LXDQApp *)qApp)->remaining);
	ui.lbTitle->setText(QString::fromWCharArray(L"<h2>请选择：</h2>"));
	ui.lbStatus->setText(QString::fromWCharArray(L"准备就绪"));
	// DLL下载线程
	DLLDownloadThread = new QThread(this);
	downloader = new DLLHandler();
	downloader->moveToThread(DLLDownloadThread);
	connect(this, SIGNAL(dodllget(QString)), downloader, SLOT(doDLLget(QString)));
	connect(downloader, SIGNAL(statusreport(wchar_t *)), this, SLOT(on_dll_status_reported(wchar_t *)));
	connect(downloader, SIGNAL(statusreport(QString, int)), this, SLOT(on_dll_status_reported(QString, int)));
	connect(downloader, SIGNAL(finished(bool)), this, SLOT(on_dll_finished(bool)));
	connect(DLLDownloadThread, SIGNAL(finished), downloader, SLOT(deleteLater));
	// DLL重命名监测线程
	DLLRenameThread = new QThread(this);
	renamer = new DLLRenamer();
	renamer->moveToThread(DLLRenameThread);
	connect(downloader, SIGNAL(DLLfilereport(QString)), renamer, SLOT(setpath(QString)));
	connect(DLLRenameThread, SIGNAL(finished), renamer, SLOT(deleteLater));
	// 托盘菜单
	traymenu = new QMenu();
	traymenu->addAction(QString::fromWCharArray(L"退出"), (qApp->quit));
	// 托盘图标
	trayicon = new QSystemTrayIcon();
	trayicon->setIcon(ico);
	trayicon->setToolTip(QString::fromWCharArray(L"洛仙都纯享版"));
	trayicon->setContextMenu(traymenu);
	connect(trayicon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(iconIsActived(QSystemTrayIcon::ActivationReason)));
	trayicon->show();
	// 启动两个线程
	DLLDownloadThread->start();
	DLLRenameThread->start();
	// 刷新DLL列表
	DLLListRefresh();
	VM_END
}

LXDLite::~LXDLite()
{
	VM_START
	QProcess* process = new QProcess;
	STR_ENCRYPT_START
	process->execute("taskkill /F /IM GTA5.exe");
	STR_ENCRYPT_END
	QIcon ico(":/LXDLite/LXDLite.ico");
	DLLDownloadThread->quit();
	DLLDownloadThread->wait();
	DLLRenameThread->quit();
	DLLRenameThread->wait();
	delete trayicon, traymenu, downloader, renamer;
	VM_END
}

void LXDLite::Exit()
{
	qApp->quit();
}

void LXDLite::iconIsActived(QSystemTrayIcon::ActivationReason reason)
{
	switch (reason)
	{
	case QSystemTrayIcon::Trigger:
	{
		this->showNormal();
		break;
	}
	case QSystemTrayIcon::DoubleClick:
	{
		this->showNormal();
		break;
	}
	default:
		break;
	}
}

void LXDLite::closeEvent(QCloseEvent *e)
{
	e->ignore();
	this->hide();
	trayicon->showMessage(QString::fromWCharArray(L"洛仙都纯享版"), QString::fromWCharArray(L"主界面已隐藏，您可以点击托盘图标来重新显示。"));
}

void LXDLite::on_lstCheats_currentIndexChanged(int index)
{
	QJsonObject o = ui.lstCheats->currentData().toJsonObject();
	QString name = o.value(tr("name")).toString();
	QString id = QString("%1").arg(o.value(tr("id")).toInt());
	QString memo = o.value(tr("memo")).toString();
	ui.lbMemo->setText(memo);
}

void LXDLite::on_btnInject_clicked()
{
	if (CheckAppRunningStatus(tr("GTA5.exe")))
	{
		ui.btnInject->setDisabled(true);
		QJsonObject o = ui.lstCheats->currentData().toJsonObject();
		QString id = QString("%1").arg(o.value(tr("id")).toInt());
		emit dodllget(id);
	}
	else
	{
		ui.lbStatus->setText(QString::fromWCharArray(L"请先运行游戏！"));
	}
}

void LXDLite::DLLListRefresh()
{
	QByteArray reqdata;
	reqdata.append("passkey=");
	reqdata.append(((LXDQApp *)qApp)->passkey);
	reqdata.append("&HWID=");
	reqdata.append(((LXDQApp *)qApp)->HWID);
	reqdata.append("&version=");
	reqdata.append(((LXDQApp *)qApp)->ver);
	QNetworkReply *rep = accessmanager.post(QNetworkRequest(QUrl("http://" + ((LXDQApp *)qApp)->host + "/passkeygetdlllist")), reqdata);
	if (rep->error() == QNetworkReply::NoError)
	{
		connect(&accessmanager, SIGNAL(finished(QNetworkReply*)), this, SLOT(on_dll_list_refreshed(QNetworkReply*)));
	}
	else
	{
		ui.lbStatus->setText(QString::fromWCharArray(L"可用外挂列表接收出错，服务器可能正在维护，请重试。"));
	}
	return;
}

void LXDLite::on_dll_list_refreshed(QNetworkReply *rep)
{
	if (rep->url().path() == QString::fromWCharArray(L"/passkeygetdlllist"))
	{
		QString respstr = rep->readAll();
		rep->deleteLater();
		QJsonDocument respjson = QJsonDocument::fromJson(respstr.toUtf8().data());
		if (respjson.isNull() || respjson.isEmpty())
		{
			ui.lbStatus->setText(QString::fromWCharArray(L"可用外挂列表接收出错，服务器可能正忙，请重试。"));
		}
		else
		{
			QJsonObject obj = respjson.object();
			QJsonValue jsonvalstatus = obj.value(QString::fromWCharArray(L"status"));
			QString status = jsonvalstatus.toString();
			if (status == QString::fromWCharArray(L"success"))
			{
				QJsonValue jsonvalpayload = obj.value(QString::fromWCharArray(L"payload"));
				QJsonValue jsonvalsessionkey = obj.value(QString::fromWCharArray(L"sessionkey"));
				// this->sessionkey = jsonvalsessionkey.toString();
				if (jsonvalpayload.isUndefined())
				{
					ui.lbStatus->setText(QString::fromWCharArray(L"可用外挂列表接收出错，服务器响应错误，请重试。"));
				}
				else
				{
					ui.lstCheats->clear();
					QJsonArray jsondlllist = jsonvalpayload.toArray();
					QJsonArray::iterator i;
					i = jsondlllist.begin();
					while (i != jsondlllist.end())
					{
						QJsonObject o = jsondlllist[i.i].toObject();
						QString name = o.value(tr("name")).toString();
						ui.lstCheats->addItem(QIcon(), name, o);
						i++;
					}
					ui.lbStatus->setText(QString::fromWCharArray(L"外挂列表已刷新！"));
				}
			}
			else
			{
				DLLListRefresh();
			}
		}

		disconnect(&accessmanager, SIGNAL(finished(QNetworkReply*)), this, SLOT(on_dll_list_refreshed(QNetworkReply*)));

	}
	return;
}

void LXDLite::on_dll_finished(bool injectcompleted)
{
	ui.btnInject->setDisabled(false);
	if (injectcompleted)
	{
		this->hide();
		trayicon->showMessage(QString::fromWCharArray(L"洛仙都客户端"), QString::fromWCharArray(L"外挂注入成功，请继续游戏吧！\n点击托盘图标显示洛仙都客户端。"));
	}
}

void LXDLite::on_dll_status_reported(wchar_t *status)
{
	ui.lbStatus->setText(QString::fromWCharArray(status));
}

void LXDLite::on_dll_status_reported(QString status, int ms)
{
	ui.lbStatus->setText(status);
}
