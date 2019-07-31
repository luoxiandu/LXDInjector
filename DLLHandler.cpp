#include "DLLHandler.h"
extern QString getRandomString(int length, bool use_symbols);
DLLHandler::DLLHandler()
	: QObject()
{
	accessmanager = new QNetworkAccessManager(this);
}

DLLHandler::~DLLHandler()
{
}

void DLLHandler::doDLLget(QString id, QString sessionkey)
{
	this->sessionkey = sessionkey;
	DLLDownload(id);
}

bool DLLHandler::Inject(QString dllpath, QByteArray xpr)
{
	VM_START
	bool result;
	emit statusreport(L"����ע�롭��");
	QString xprstr(xpr);
	xprstr.replace("#FUCKINGFILEPATH#", dllpath);
	QFile tmpxpr("LuoXianDu.xpr64");
	if (tmpxpr.open(QIODevice::WriteOnly))
	{
		tmpxpr.write(xprstr.toUtf8());
		QFileInfo xprinfo(tmpxpr);
		tmpxpr.close();
		QProcess *Xenos = new QProcess(this);
		Xenos->start("Xenos64.exe", QStringList() << "--run" << xprinfo.filePath());
		result = Xenos->waitForFinished();
		if (!result)
		{
			emit showerr(Xenos->errorString() + "\n" +  Xenos->readAllStandardError());
		}
	}
	else
	{
		emit showerr(tmpxpr.errorString());
		emit statusreport(L"�ļ�д��ʧ�ܣ������ԡ�");
		result = false;
	}
	QFile tmpdll(dllpath);
	tmpdll.remove();
	tmpxpr.remove();
	return result;
	VM_END
}

void DLLHandler::DLLDownload(QString id)
{
	QByteArray reqdata;
	reqdata.append("id=");
	reqdata.append(id);
	reqdata.append("&sessionkey=");
	reqdata.append(this->sessionkey);
	reqdata.append("&version=");
	reqdata.append(((LXDQApp *)qApp)->ver);
	QNetworkReply *rep = accessmanager->post(QNetworkRequest(QUrl("http://" + ((LXDQApp *)qApp)->host + "/getdll")), reqdata);
	if (rep->error() == QNetworkReply::NoError)
	{
		connect(rep, SIGNAL(downloadProgress(qint64, qint64)), this, SLOT(on_progress_reported(qint64, qint64)));
		connect(accessmanager, SIGNAL(finished(QNetworkReply*)), this, SLOT(on_dll_downloaded(QNetworkReply*)));
		downloadclock = QTime::currentTime();
		lastrec = 0;
	}
	else
	{
		emit statusreport(L"���س�����������������ά���������ԡ�");
	}
	return;
}

void DLLHandler::on_dll_downloaded(QNetworkReply *rep)
{
	VM_START
	bool injectcompleted = false;
	if (rep->url().path() == QString::fromWCharArray(L"/getdll"))
	{
		QString respstr = rep->readAll();
		rep->deleteLater();
		QJsonDocument respjson = QJsonDocument::fromJson(respstr.toUtf8().data());
		if (respjson.isNull() || respjson.isEmpty())
		{
			emit statusreport(L"���س���������������æ�������ԡ�");
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
					emit statusreport(L"���س�����������Ӧ���������ԡ�");
				}
				else
				{
					QJsonObject injectobj = jsonvalpayload.toObject();
					QJsonValue dllpayload = injectobj.value(tr("dll"));
					QJsonValue xprpayload = injectobj.value(tr("xpr"));
					QJsonValue rcpayload = injectobj.value(tr("resource"));
					QJsonValue keypayload = injectobj.value(tr("keyfile"));
					QJsonValue keynamepayload = injectobj.value(tr("keyfilename"));
					QByteArray dllfile = QByteArray::fromBase64(dllpayload.toString().toLatin1());
					QByteArray xprfile = QByteArray::fromBase64(xprpayload.toString().toLatin1());
					if (!rcpayload.isUndefined())
					{
						QSettings GTAreg("HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\Steam App 271590", QSettings::NativeFormat);
						QString GTAdir = GTAreg.value("InstallLocation").toString();
						QString fname = GTAdir + "\\rcinst.exe";
						QFile tmprcinst(fname);
						if (tmprcinst.open(QIODevice::WriteOnly))
						{
							QByteArray rcfile = QByteArray::fromBase64(rcpayload.toString().toLatin1());
							tmprcinst.write(rcfile);
							bool fuck = tmprcinst.setPermissions(QFileDevice::ExeOwner);
							tmprcinst.flush();
							tmprcinst.close();
							QFileInfo tmprcinfo(tmprcinst);
							QProcess* process = new QProcess(this);	
							process->setWorkingDirectory(GTAdir);
							process->start(fname, QStringList());
							if (process->waitForFinished())
							{
								emit statusreport(L"�����ļ���װ�ɹ�");
							}
							else
							{
								emit statusreport(L"�����ļ���װ�������⣬���ܻ���ʾ����");
							}
							tmprcinst.remove();
						}
						else
						{
							emit showerr(tmprcinst.errorString());
							emit statusreport(L"�����ļ���װ��д��ʧ�ܣ������ԡ�");
						}
					}
					QString dllname = QDir::tempPath() + "/" + getRandomString((rand() % 10) + 5, false) + ".bin";
					QFile tmpdll(dllname);
					if (tmpdll.open(QIODevice::WriteOnly))
					{
						if (!keypayload.isUndefined())
						{
							QString keyname = QDir::tempPath() + "/" + keynamepayload.toString();
							QFile tmpkey(keyname);
							if (tmpkey.open(QIODevice::WriteOnly))
							{
								QByteArray keyfile = QByteArray::fromBase64(keypayload.toString().toLatin1());
								tmpkey.write(keyfile);
								tmpkey.close();
							}
						}
						tmpdll.write(dllfile);
						QFileInfo dllinfo(tmpdll);
						tmpdll.close();
						if (injectcompleted = Inject(dllinfo.absoluteFilePath(), xprfile))
						{
							emit statusreport(L"ע����ɣ��뿪ʼ��Ϸ�ɣ�");
						}
						else
						{
							emit statusreport(L"ע��ʧ�ܣ������ԡ�");
						}
						//��ʼ��������ļ�����Ȼ��GTAռ��û��ɾ�������ǿ��Ը���
						QString newdllname = QDir::tempPath() + "/" + getRandomString((rand() % 10) + 20, true);
						tmpdll.rename(newdllname);
						QFileInfo newdllinfo(tmpdll);
						if (!keypayload.isUndefined())
						{
							QString keyname = QDir::tempPath() + "/" + keynamepayload.toString();
							QFile tmpkey(keyname);
							tmpkey.remove();
						}
						QFile xenoslog("Xenos.log");
						xenoslog.remove();
						emit DLLfilereport(newdllinfo.absoluteFilePath());
					}
					else
					{
						emit showerr(tmpdll.errorString());
						emit statusreport(L"��ұ����ļ�д��ʧ�ܣ������ԡ�");
					}
				}
			}
			else
			{
				emit statusreport(L"��ʱ��������������ԡ�");
			}
		}
		disconnect(accessmanager, SIGNAL(finished(QNetworkReply*)), this, SLOT(on_dll_downloaded(QNetworkReply*)));
		disconnect(rep, SIGNAL(downloadProgress(qint64, qint64)), this, SLOT(on_progress_reported(qint64, qint64)));
	}
	emit finished(injectcompleted);
	return;
	VM_END
}

void DLLHandler::on_progress_reported(qint64 rec, qint64 tot)
{
	int intervaltime = downloadclock.msecsTo(QTime::currentTime());
	int intervalrec = rec - lastrec;
	QString status;
	status.append(QString::fromWCharArray(L"���ս���:"));
	if (rec >= 1048576)
		status.append(tr("%1MB").arg(rec / 1048576.0));
	else if (rec < 1048576 && rec > 1024)
		status.append(tr("%1KB").arg(rec / 1024.0));
	else
		status.append(tr("%1Bytes").arg(rec));
	status.append(QString::fromWCharArray(L" / "));
	if (tot >= 1048576)
		status.append(tr("%1MB").arg(tot / 1048576.0));
	else if (tot < 1048576 && tot > 1024)
		status.append(tr("%1KB").arg(tot / 1024.0));
	else
		status.append(tr("%1Bytes").arg(tot));
	qreal bytespersecond = ((intervalrec * 1000.0) / (qreal)intervaltime);
	status.append(QString::fromWCharArray(L"   �����ٶȣ�"));
	if (bytespersecond >= 1048576)
		status.append(tr("%1MB/s").arg(bytespersecond / 1048576.0));
	else if (bytespersecond < 1048576 && bytespersecond > 1024)
		status.append(tr("%1KB/s").arg(bytespersecond / 1024.0));
	else
		status.append(tr("%1B/s").arg(bytespersecond));
	emit statusreport(status, 1000);
	downloadclock = QTime::currentTime();
	lastrec = rec;
}