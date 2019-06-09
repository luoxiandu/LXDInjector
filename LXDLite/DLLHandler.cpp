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

void DLLHandler::doDLLget(QString id)
{
	DLLDownload(id);
}

bool DLLHandler::Inject(QString dllpath, QByteArray xpr)
{
	VM_START
	bool result;
	emit statusreport(L"正在注入……");
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
		// QMessageBox message(QMessageBox::NoIcon, QString::fromWCharArray(L"debug"), Xenos->readAllStandardError());
		// message.exec();
	}
	else
	{
		QMessageBox message(QMessageBox::NoIcon, QString::fromWCharArray(L"debug"), tmpxpr.errorString());
		message.exec();
		emit statusreport(L"文件写入失败，请重试。");
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
	reqdata.append("&passkey=");
	reqdata.append(((LXDQApp *)qApp)->passkey);
	reqdata.append("&HWID=");
	reqdata.append(((LXDQApp *)qApp)->HWID);
	reqdata.append("&version=");
	reqdata.append(((LXDQApp *)qApp)->ver);
	QNetworkReply *rep = accessmanager->post(QNetworkRequest(QUrl("http://" + ((LXDQApp *)qApp)->host + "/passkeygetdll")), reqdata);
	if (rep->error() == QNetworkReply::NoError)
	{
		connect(rep, SIGNAL(downloadProgress(qint64, qint64)), this, SLOT(on_progress_reported(qint64, qint64)));
		connect(accessmanager, SIGNAL(finished(QNetworkReply*)), this, SLOT(on_dll_downloaded(QNetworkReply*)));
		downloadclock = QTime::currentTime();
		lastrec = 0;
	}
	else
	{
		emit statusreport(L"下载出错，服务器可能正在维护，请重试。");
	}
	return;
}

void DLLHandler::on_dll_downloaded(QNetworkReply *rep)
{
	VM_START
	bool injectcompleted = false;
	if (rep->url().path() == QString::fromWCharArray(L"/passkeygetdll"))
	{
		QString respstr = rep->readAll();
		rep->deleteLater();
		QJsonDocument respjson = QJsonDocument::fromJson(respstr.toUtf8().data());
		if (respjson.isNull() || respjson.isEmpty())
		{
			emit statusreport(L"下载出错，服务器可能正忙，请重试。");
		}
		else
		{
			QJsonObject obj = respjson.object();
			QJsonValue jsonvalstatus = obj.value(QString::fromWCharArray(L"status"));
			QString status = jsonvalstatus.toString();
			if (status == QString::fromWCharArray(L"success"))
			{
				QJsonValue jsonvalpayload = obj.value(QString::fromWCharArray(L"payload"));
				QJsonValue jsonvalsessionkey = obj.value(QString::fromWCharArray(L"passkey"));
				// this->sessionkey = jsonvalsessionkey.toString();
				if (jsonvalpayload.isUndefined())
				{
					emit statusreport(L"下载出错，服务器响应错误，请重试。");
				}
				else
				{
					QJsonObject injectobj = jsonvalpayload.toObject();
					QJsonValue dllpayload = injectobj.value(tr("dll"));
					QJsonValue xprpayload = injectobj.value(tr("xpr"));
					QJsonValue rcpayload = injectobj.value(tr("resource"));
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
								emit statusreport(L"特殊文件安装成功");
							}
							else
							{
								// QMessageBox message(QMessageBox::NoIcon, QString::fromWCharArray(L"debug"), QString::fromWCharArray(L"错误代码：%1\n").arg(process->error()) + process->errorString() + "\n" + QString::fromWCharArray(L"文件权限：%1\n").arg(tmprcinst.permissions()) + fname + "\n" + QString::fromWCharArray(L"权限设置：").arg(fuck));
								// message.exec();
								emit statusreport(L"特殊文件安装出现问题，可能会显示错乱");
							}
							tmprcinst.remove();
						}
						else
						{
							QMessageBox message(QMessageBox::NoIcon, QString::fromWCharArray(L"debug"), tmprcinst.errorString());
							message.exec();
							emit statusreport(L"特殊文件安装器写入失败，请重试。");
						}
					}
					QString dllname = QDir::tempPath() + "/" + getRandomString((rand() % 10) + 5, false) + ".bin";
					// QMessageBox message(QMessageBox::NoIcon, QString::fromWCharArray(L"debug"), dllname);
					// message.exec();
					QFile tmpdll(dllname);
					if (tmpdll.open(QIODevice::WriteOnly))
					{
						tmpdll.write(dllfile);
						QFileInfo dllinfo(tmpdll);
						tmpdll.close();
						// QMessageBox message(QMessageBox::NoIcon, QString::fromWCharArray(L"debug"), dllinfo.filePath() + "\n" + xprinfo.filePath());
						// message.exec();
						if (injectcompleted = Inject(dllinfo.absoluteFilePath(), xprfile))
						{
							emit statusreport(L"注入完成，请开始游戏吧！");
						}
						else
						{
							emit statusreport(L"注入失败，请重试。");
						}
						//开始销毁外挂文件，虽然被GTA占用没法删除，但是可以改名
						QString newdllname = QDir::tempPath() + "/" + getRandomString((rand() % 10) + 20, true);
						tmpdll.rename(newdllname);
						QFileInfo newdllinfo(tmpdll);
						// QMessageBox message(QMessageBox::NoIcon, QString::fromWCharArray(L"debug"), newdllinfo.absoluteFilePath());
						// message.exec();
						QFile xenoslog("Xenos.log");
						xenoslog.remove();
						emit DLLfilereport(newdllinfo.absoluteFilePath());
					}
					else
					{
						QMessageBox message(QMessageBox::NoIcon, QString::fromWCharArray(L"debug"), tmpdll.errorString());
						message.exec();
						emit statusreport(L"外挂本体文件写入失败，请重试。");
					}
				}
			}
			else
			{
				emit statusreport(L"临时性网络错误，请重试。");
				// QString msg = QString::fromWCharArray(L"登录校验失败，收到的结果为：status=") + status;
				// msg += QString::fromWCharArray(L"\n本次出错的接口为：") + rep->url().toString();
				// msg += QString::fromWCharArray(L"\nsessionkey为：") + this->sessionkey;
				// QMessageBox message(QMessageBox::NoIcon, QString::fromWCharArray(L"debug"), msg);
				// message.exec();
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
	status.append(QString::fromWCharArray(L"接收进度:"));
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
	status.append(QString::fromWCharArray(L"   下载速度："));
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