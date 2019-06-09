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
	setWindowTitle(QString::fromWCharArray(L"���ɶ������ ����ʣ����Ч�ڣ�") + ((LXDQApp *)qApp)->remaining);
	ui.lbTitle->setText(QString::fromWCharArray(L"<h2>��ѡ��</h2>"));
	ui.lbStatus->setText(QString::fromWCharArray(L"׼������"));
	// DLL�����߳�
	DLLDownloadThread = new QThread(this);
	downloader = new DLLHandler();
	downloader->moveToThread(DLLDownloadThread);
	connect(this, SIGNAL(dodllget(QString)), downloader, SLOT(doDLLget(QString)));
	connect(downloader, SIGNAL(statusreport(wchar_t *)), this, SLOT(on_dll_status_reported(wchar_t *)));
	connect(downloader, SIGNAL(statusreport(QString, int)), this, SLOT(on_dll_status_reported(QString, int)));
	connect(downloader, SIGNAL(finished(bool)), this, SLOT(on_dll_finished(bool)));
	connect(DLLDownloadThread, SIGNAL(finished), downloader, SLOT(deleteLater));
	// DLL����������߳�
	DLLRenameThread = new QThread(this);
	renamer = new DLLRenamer();
	renamer->moveToThread(DLLRenameThread);
	connect(downloader, SIGNAL(DLLfilereport(QString)), renamer, SLOT(setpath(QString)));
	connect(DLLRenameThread, SIGNAL(finished), renamer, SLOT(deleteLater));
	// ���̲˵�
	traymenu = new QMenu();
	traymenu->addAction(QString::fromWCharArray(L"�˳�"), (qApp->quit));
	// ����ͼ��
	trayicon = new QSystemTrayIcon();
	trayicon->setIcon(ico);
	trayicon->setToolTip(QString::fromWCharArray(L"���ɶ������"));
	trayicon->setContextMenu(traymenu);
	connect(trayicon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(iconIsActived(QSystemTrayIcon::ActivationReason)));
	trayicon->show();
	// ���������߳�
	DLLDownloadThread->start();
	DLLRenameThread->start();
	// ˢ��DLL�б�
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
	trayicon->showMessage(QString::fromWCharArray(L"���ɶ������"), QString::fromWCharArray(L"�����������أ������Ե������ͼ����������ʾ��"));
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
		ui.lbStatus->setText(QString::fromWCharArray(L"����������Ϸ��"));
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
		ui.lbStatus->setText(QString::fromWCharArray(L"��������б���ճ�����������������ά���������ԡ�"));
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
			ui.lbStatus->setText(QString::fromWCharArray(L"��������б���ճ���������������æ�������ԡ�"));
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
					ui.lbStatus->setText(QString::fromWCharArray(L"��������б���ճ�����������Ӧ���������ԡ�"));
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
					ui.lbStatus->setText(QString::fromWCharArray(L"����б���ˢ�£�"));
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
		trayicon->showMessage(QString::fromWCharArray(L"���ɶ��ͻ���"), QString::fromWCharArray(L"���ע��ɹ����������Ϸ�ɣ�\n�������ͼ����ʾ���ɶ��ͻ��ˡ�"));
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
