#include "stdafx.h"
#include "LXDInjector.h"

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

QString getRandomString(int length, bool use_symbols)
{
	qsrand(QDateTime::currentMSecsSinceEpoch());//Ϊ���ֵ�趨һ��seed

	const char* chrs;
	if(!use_symbols)
		chrs = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
	else
		chrs = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!@#$%^&()_+-=[]{}";
	int chrs_size = strlen(chrs);

	char* ch = new char[length + 1];
	memset(ch, 0, length + 1);
	int randomx = 0;
	for (int i = 0; i < length; ++i)
	{
		randomx = rand() % (chrs_size - 1);
		ch[i] = chrs[randomx];
	}

	QString ret(ch);
	delete[] ch;
	return ret;
}


LXDInjector::LXDInjector(QWidget *parent)
	: QMainWindow(parent)
{
	this->setWindowFlag(Qt::MSWindowsFixedSizeDialogHint, true);
	QIcon ico(":/LXDInjector/LXDInjector.ico");
	setWindowIcon(ico);
	ui.setupUi(this);
	settings = new QSettings(QSettings::Registry64Format, QSettings::UserScope, QString::fromWCharArray(L"LuoXianDu"), QString::fromWCharArray(L"LXDInjector"));
	accessmanager = new QNetworkAccessManager(this);
	setWindowTitle(QString::fromWCharArray(L"���ɶ� V") + ((LXDQApp *)qApp)->ver);
	this->sessionkey = ((LXDQApp *)qApp)->sessionkey;
	this->refreshTimer = new QTimer(this);
	this->chksocket = new QWebSocket();
	connect(this->chksocket, SIGNAL(connected()), this, SLOT(wsconnected()));
	connect(this->chksocket, SIGNAL(disconnected()), this, SLOT(wsdisconnected()));
	connect(this->chksocket, SIGNAL(textMessageReceived(QString)), this, SLOT(wsrecieved(QString)));
	connect(this->refreshTimer, SIGNAL(timeout()), this, SLOT(chklogin()));
	// DLL�����߳�
	DLLDownloadThread = new QThread(this);
	downloader = new DLLHandler();
	downloader->moveToThread(DLLDownloadThread);
	connect(this, SIGNAL(dodllget(QString, QString)), downloader, SLOT(doDLLget(QString, QString)));
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
	traymenu->addAction(ui.actionExit);
	// ����ͼ��
	trayicon = new QSystemTrayIcon();
	trayicon->setIcon(ico);
	trayicon->setToolTip(QString::fromWCharArray(L"���ɶ��ͻ���"));
	trayicon->setContextMenu(traymenu);
	connect(trayicon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(iconIsActived(QSystemTrayIcon::ActivationReason)));
	trayicon->show();
	// �����ػ�����
	if (!QProcess::startDetached("LXDGuard.exe"))
	{
		QMessageBox message(QMessageBox::NoIcon, QString::fromWCharArray(L"����"), QString::fromWCharArray(L"��Ҫ��������ʧ�ܣ������Զ��˳���"));
		message.setWindowIcon(ico);
		message.exec();
		exit(0);
	}
	// �����ػ�����
	while (!CheckAppRunningStatus("LXDGuard.exe"));
	GuardSocket.connectToServer("LXDGuardPipe");
	if (!GuardSocket.waitForConnected(-1))
	{
		QMessageBox message(QMessageBox::NoIcon, QString::fromWCharArray(L"����"), QString::fromWCharArray(L"��Ҫ��������ʧ�ܣ������Զ��˳���"));
		message.setWindowIcon(ico);
		message.exec();
		exit(0);
	}
	connect(&GuardSocket, SIGNAL(disconnected()), this, SLOT(on_guard_died()));
	// ���������߳�
	DLLDownloadThread->start();
	DLLRenameThread->start();
	// ˢ���û���Ϣ
	AccountInfoRefresh();
	// ������֤
	this->chksocket->open(QUrl("ws://" + ((LXDQApp *)qApp)->host + "/chklogin/" + this->sessionkey.split("::")[0]));
}

LXDInjector::~LXDInjector()
{
	VM_START
	QProcess* process = new QProcess;
	STR_ENCRYPT_START
	process->execute("taskkill /F /IM GTA5.exe");
	STR_ENCRYPT_END
	QIcon ico(":/LXDInjector/LXDInjector.ico");
	STR_ENCRYPTW_START
	if (this->loginoutdated)
	{
		QMessageBox message(QMessageBox::NoIcon, QString::fromWCharArray(L"����"), QString::fromWCharArray(L"��¼ʧЧ���ڣ��������˳���\n�����ǵ�¼�û�����ȷ�������˺�û��ͬʱ�࿪����ͻ��ˡ�"));
		message.setWindowIcon(ico);
		message.exec();
	}
	if (this->networkerrorcount)
	{
		QMessageBox message(QMessageBox::NoIcon, QString::fromWCharArray(L"����"), QString::fromWCharArray(L"�������ʧȥ���ӣ������Զ��˳���"));
		message.setWindowIcon(ico);
		message.exec();
	}
	if (this->isBeggar)
	{
		QMessageBox message(QMessageBox::NoIcon, QString::fromWCharArray(L"��ʾ"), QString::fromWCharArray(L"<a style=\"color: #BBBBBB\" href=\"https://shang.qq.com/wpa/qunwpa?idkey=c9f0a58ddf654dfee69356d744d426607f5f2ed076c09c1be78e5af2a4f893a8\">�������ɶ�QQȺ��105976356��</a>�ɻ�ò���ʱ��������Ŷ~"));
		message.setWindowIcon(ico);
		message.exec();
	}
	STR_ENCRYPTW_END
	DLLDownloadThread->quit();
	DLLDownloadThread->wait();
	DLLRenameThread->quit();
	DLLRenameThread->wait();
	delete trayicon, traymenu, downloader, renamer;
	VM_END
}

void LXDInjector::on_guard_died()
{
	VM_START
	STR_ENCRYPT_START
	QProcess* process = new QProcess;
	process->execute("taskkill /F /IM GTA5.exe");
	disconnect(&GuardSocket, SIGNAL(disconnected()), this, SLOT(on_guard_died()));
	qApp->quit();
	STR_ENCRYPT_END
	VM_END
}

void LXDInjector::showError(QString errstr)
{
	QIcon ico(":/LXDInjector/LXDInjector.ico");
	QMessageBox message(QMessageBox::Warning, QString::fromWCharArray(L"����"), errstr);
	message.setWindowIcon(ico);
	message.exec();
}

void LXDInjector::iconIsActived(QSystemTrayIcon::ActivationReason reason)
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

void LXDInjector::closeEvent(QCloseEvent *e)
{
	e->ignore();
	this->hide();
	trayicon->showMessage(QString::fromWCharArray(L"���ɶ��ͻ���"), QString::fromWCharArray(L"�����������أ������Ե������ͼ����������ʾ��"));
}

void LXDInjector::wsconnected()
{
	this->refreshTimer->start(3000);
	this->networkerrorcount = 0;
}

void LXDInjector::wsdisconnected()
{
	this->refreshTimer->stop();
	if(++this->networkerrorcount == 1)
		QApplication::quit();
}

void LXDInjector::chklogin()
{
	this->chksocket->sendTextMessage(sessionkey);
}

void LXDInjector::wsrecieved(QString rep)
{
	if (rep.contains("*failed*"))
	{
		this->refreshTimer->stop();
		this->loginoutdated = true;
		QApplication::quit();
	}
	else
	{
		this->sessionkey = rep;
	}
}

void LXDInjector::setStatus(wchar_t *status)
{
	QString s = QString::fromWCharArray(status);
	ui.statusBar->showMessage(s, 5000);
}

void LXDInjector::on_actionAbout_triggered()
{
	VM_START
	QIcon ico(":/LXDInjector/LXDInjector.ico");
	STR_ENCRYPTW_START
	QString msg = QString::fromWCharArray(L"<p>���ɶ��ͻ���</p>");
	msg += QString::fromWCharArray(L"<p>���ɶ�è������</p>");
	msg += QString::fromWCharArray(L"<p>����&nbsp;<a style=\"color: #BBBBBB\" href=\"https://www.qt.io/\">Qt</a>&nbsp;����<br>ʹ��&nbsp;<a style=\"color: #BBBBBB\" href=\"https://github.com/DarthTon/Xenos\">Xenos</a>&nbsp;�ڴ�ע�뼼��</p>");
	msg += QString::fromWCharArray(L"<p>���ɶ���ӭ��<br>һȺ��105976356<br>��Ⱥ��1026775748</p>");
	QMessageBox message(QMessageBox::Information, QString::fromWCharArray(L"����"), msg);
	STR_ENCRYPTW_END
	message.setWindowIcon(ico);
	message.setIconPixmap(QPixmap(":/LXDInjector/cat.png"));
	message.exec();
	return;
	VM_END
}

void LXDInjector::on_actionHelp_triggered()
{
	QIcon ico(":/LXDInjector/LXDInjector.ico");
	QMessageBox message(QMessageBox::Information, QString::fromWCharArray(L"ʹ��˵��"), QString::fromWCharArray(L"ѡ����Һ����Ϸ���ע�뼴��һ�����ң�"));
	message.setWindowIcon(ico);
	message.exec();
	return;
}

void LXDInjector::on_actionExit_triggered()
{
	QApplication::quit();
}

void LXDInjector::on_actionRefresh_triggered()
{
	DLLListRefresh();
}

void LXDInjector::on_actionRefreshAccount_triggered()
{
	AccountInfoRefresh();
}

void LXDInjector::on_actionDeposit_triggered()
{
	if (!isBeggar)
	{
		Deposit w;
		w.exec();
		AccountInfoRefresh();
	}
	else
	{
		this->setStatus(L"��������ȫ��ѣ������ֵ��");
	}
}

void LXDInjector::on_actionGameAccountService_triggered()
{
	if (!isBeggar)
	{
		frmGameAccountService w;
		w.exec();
		AccountInfoRefresh();
	}
	else
	{
		this->setStatus(L"�����û��޷�����ٷ��˺ŷ���");
	}
}

void LXDInjector::on_actiongetRockstarStatus_triggered()
{
	RockstarStatusThread = new QThread(this);
	RockstarStatusworker = new RockstarStatus();
	RockstarStatusworker->moveToThread(RockstarStatusThread);
	connect(RockstarStatusworker, SIGNAL(finishget(QString)), this, SLOT(on_RockstarStatus_got(QString)));
	connect(this, SIGNAL(doRockstarStatusget()), RockstarStatusworker, SLOT(doget()));
	connect(RockstarStatusworker, SIGNAL(reporterr(QString)), this, SLOT(showError(QString)));
	connect(RockstarStatusThread, SIGNAL(finished), RockstarStatusworker, SLOT(deleteLater));
	RockstarStatusThread->start();
	this->setStatus(L"���ڲ�ѯ����");
	emit doRockstarStatusget();
}

void LXDInjector::on_RockstarStatus_got(QString result)
{
	disconnect(RockstarStatusworker, SIGNAL(finishget(QString)), this, SLOT(on_RockstarStatus_got(QString)));
	disconnect(this, SIGNAL(doRockstarStatusget()), RockstarStatusworker, SLOT(doget()));
	disconnect(RockstarStatusworker, SIGNAL(reporterr(QString)), this, SLOT(showError(QString)));
	delete RockstarStatusworker;
	RockstarStatusThread->quit();
	RockstarStatusThread->wait();
	delete RockstarStatusThread;
	QIcon ico(":/LXDInjector/LXDInjector.ico");
	QMessageBox message(QMessageBox::Information, QString::fromWCharArray(L"Rockstar������״̬"), result);
	message.setWindowIcon(ico);
	message.exec();
}

void LXDInjector::on_btnInject_clicked()
{
	VM_START
	STR_ENCRYPT_START
	if (CheckAppRunningStatus(tr("GTA5.exe")))
	{
		STR_ENCRYPT_END
		ui.btnInject->setDisabled(true);
		QJsonObject o = ui.lstCheats->currentData().toJsonObject();
		QString id = QString("%1").arg(o.value(tr("id")).toInt());
		emit dodllget(id, this->sessionkey);
	}
	else
	{
		this->setStatus(L"����������Ϸ��");
	}
	VM_END
}

void LXDInjector::on_btnTranslate_clicked()
{
	this->setStatus(L"����ͼ�������ڿ�����");
}

void LXDInjector::on_lstCheats_currentIndexChanged(int index)
{
	QJsonObject o = ui.lstCheats->currentData().toJsonObject();
	QString name = o.value(tr("name")).toString();
	QString id = QString("%1").arg(o.value(tr("id")).toInt());
	QString memo = o.value(tr("memo")).toString();
	ui.lbMemo->setText(memo);
}

void LXDInjector::on_dll_finished(bool injectcompleted)
{
	ui.btnInject->setDisabled(false);
	if (injectcompleted)
	{
		this->hide();
		trayicon->showMessage(QString::fromWCharArray(L"���ɶ��ͻ���"), QString::fromWCharArray(L"���ע��ɹ����������Ϸ�ɣ�\n�������ͼ����ʾ���ɶ��ͻ��ˡ�"));
	}
}

void LXDInjector::on_dll_status_reported(QString status, int ms)
{
	ui.statusBar->showMessage(status, ms);
}

void LXDInjector::on_dll_status_reported(wchar_t *message)
{
	this->setStatus(message);
}

void LXDInjector::on_progress_reported(qint64 rec, qint64 tot)
{
	QString status;
	status.append(QString::fromWCharArray(L"���ս���:"));
	if(rec >= 1048576)
		status.append(tr("%1MB").arg(rec / 1048576.0));
	else if(rec < 1048576 && rec > 1024)
		status.append(tr("%1KB").arg(rec / 1024.0));
	else
		status.append(tr("%1Bytes").arg(rec));
	status.append(QString::fromWCharArray(L" / "));
	if(tot >= 1048576)
		status.append(tr("%1MB").arg(tot / 1048576.0));
	else if(tot < 1048576 && tot > 1024)
		status.append(tr("%1KB").arg(tot / 1024.0));
	else
		status.append(tr("%1Bytes").arg(tot));
	ui.statusBar->showMessage(status, 1000);
}

void LXDInjector::DLLListRefresh()
{
	QByteArray reqdata;
	reqdata.append("sessionkey=");
	reqdata.append(this->sessionkey);
	reqdata.append("&version=");
	reqdata.append(((LXDQApp *)qApp)->ver);
	QNetworkReply *rep = accessmanager->post(QNetworkRequest(QUrl("http://" + ((LXDQApp *)qApp)->host + "/getdlllist")), reqdata);
	if (rep->error() == QNetworkReply::NoError)
	{
		// connect(rep, SIGNAL(downloadProgress(qint64, qint64)), this, SLOT(on_progress_reported(qint64, qint64)));
		connect(accessmanager, SIGNAL(finished(QNetworkReply*)), this, SLOT(on_dll_list_refreshed(QNetworkReply*)));
	}
	else
	{
		this->setStatus(L"��������б���ճ�����������������ά���������ԡ�");
	}
	return;
}

void LXDInjector::AccountInfoRefresh()
{
	QByteArray reqdata;
	reqdata.append("sessionkey=");
	reqdata.append(this->sessionkey);
	reqdata.append("&version=");
	reqdata.append(((LXDQApp *)qApp)->ver);
	QNetworkReply *rep = accessmanager->post(QNetworkRequest(QUrl("http://" + ((LXDQApp *)qApp)->host + "/getaccountinfo")), reqdata);
	if (rep->error() == QNetworkReply::NoError)
	{
		// connect(rep, SIGNAL(downloadProgress(qint64, qint64)), this, SLOT(on_progress_reported(qint64, qint64)));
		connect(accessmanager, SIGNAL(finished(QNetworkReply*)), this, SLOT(on_account_info_refreshed(QNetworkReply*)));
	}
	else
	{
		QMessageBox message(QMessageBox::NoIcon, QString::fromWCharArray(L"����"), QString::fromWCharArray(L"�˻���Ϣ�����볢����������"));
		message.exec();
		QApplication::quit();
	}
	return;
}

void LXDInjector::on_dll_list_refreshed(QNetworkReply *rep)
{
	if (rep->url().path() == QString::fromWCharArray(L"/getdlllist"))
	{
		QString respstr = rep->readAll();
		rep->deleteLater();
		QJsonDocument respjson = QJsonDocument::fromJson(respstr.toUtf8().data());
		if (respjson.isNull() || respjson.isEmpty())
		{
			this->setStatus(L"��������б���ճ���������������æ�������ԡ�");
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
					this->setStatus(L"��������б���ճ�����������Ӧ���������ԡ�");
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
					this->setStatus(L"����б���ˢ�£�");
				}
			}
			else
			{
				DLLListRefresh();
				// QString msg = QString::fromWCharArray(L"��¼У��ʧ�ܣ��յ��Ľ��Ϊ��status=") + status;
				// msg += QString::fromWCharArray(L"\n���γ���Ľӿ�Ϊ��") + rep->url().toString();
				// msg += QString::fromWCharArray(L"\nsessionkeyΪ��") + this->sessionkey;
				// QMessageBox message(QMessageBox::NoIcon, QString::fromWCharArray(L"debug"), msg);
				// message.exec();
				// this->loginoutdated = true;
				// QApplication::quit();
			}
		}

		disconnect(accessmanager, SIGNAL(finished(QNetworkReply*)), this, SLOT(on_dll_list_refreshed(QNetworkReply*)));
		// disconnect(rep, SIGNAL(downloadProgress(qint64, qint64)), this, SLOT(on_progress_reported(qint64, qint64)));

	}
	return;
}

void LXDInjector::on_account_info_refreshed(QNetworkReply *rep)
{
	if (rep->url().path() == QString::fromWCharArray(L"/getaccountinfo"))
	{
		QString respstr = rep->readAll();
		rep->deleteLater();
		QJsonDocument respjson = QJsonDocument::fromJson(respstr.toUtf8().data());
		if (respjson.isNull() || respjson.isEmpty())
		{
			QMessageBox message(QMessageBox::NoIcon, QString::fromWCharArray(L"����"), QString::fromWCharArray(L"�˻���Ϣ�����볢����������"));
			message.exec();
			QApplication::quit();
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
					QMessageBox message(QMessageBox::NoIcon, QString::fromWCharArray(L"����"), QString::fromWCharArray(L"�˻���Ϣ�����볢����������"));
					message.exec();
					QApplication::quit();
				}
				else
				{
					QJsonObject payloadobj = jsonvalpayload.toObject();
					double balance = payloadobj.value(QString::fromWCharArray(L"balance")).toDouble();
					this->isBeggar = payloadobj.value(QString::fromWCharArray(L"isBeggar")).toBool();
					((LXDQApp *)qApp)->prices = payloadobj.value(QString::fromWCharArray(L"prices")).toObject();
					QString t;
					if(!isBeggar)
						t = QString::fromWCharArray(L"���ɶ� V") + ((LXDQApp *)qApp)->ver + QString::fromWCharArray(L" ��¼�û���") + sessionkey.split("::")[0] + QString::fromWCharArray(L" ��%1Ԫ").arg(balance);
					else
						t = QString::fromWCharArray(L"���ɶ� V") + ((LXDQApp *)qApp)->ver + QString::fromWCharArray(L" ���ð�");
					setWindowTitle(t);
					this->setStatus(L"�˻���Ϣ��ˢ�£�");
				}
			}
			else
			{
				AccountInfoRefresh();
				// QString msg = QString::fromWCharArray(L"��¼У��ʧ�ܣ��յ��Ľ��Ϊ��status=") + status;
				// msg += QString::fromWCharArray(L"\n���γ���Ľӿ�Ϊ��") + rep->url().toString();
				// msg += QString::fromWCharArray(L"\nsessionkeyΪ��") + this->sessionkey;
				// QMessageBox message(QMessageBox::NoIcon, QString::fromWCharArray(L"debug"), msg);
				// message.exec();
				// this->loginoutdated = true;
				// QApplication::quit();
			}
		}

		disconnect(accessmanager, SIGNAL(finished(QNetworkReply*)), this, SLOT(on_account_info_refreshed(QNetworkReply*)));
		// disconnect(rep, SIGNAL(downloadProgress(qint64, qint64)), this, SLOT(on_progress_reported(qint64, qint64)));
	}
	if (isFirstRun)
	{
		DLLListRefresh();
		isFirstRun = false;
	}
	return;
}
