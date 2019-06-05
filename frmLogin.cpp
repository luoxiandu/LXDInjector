#include "stdafx.h"
#include "frmLogin.h"
#include "LXDQApp.h"

frmLogin::frmLogin(QWidget *parent)
	: QDialog(parent)
{
	QIcon ico(":/LXDInjector/LXDInjector.ico");
	setWindowIcon(ico);
	ui.setupUi(this);
	accessmanager = new QNetworkAccessManager(this);
	settings = new QSettings(QSettings::Registry64Format, QSettings::UserScope, QString::fromWCharArray(L"LuoXianDu"), QString::fromWCharArray(L"LXDInjector"));
	connect(accessmanager, SIGNAL(finished(QNetworkReply*)), this, SLOT(on_login_recieved(QNetworkReply*)));
	ui.lnPassword->setEchoMode(QLineEdit::Password);
	if (settings->value("username").isValid() && settings->value("password").isValid())
	{
		ui.cbRemember->setChecked(true);
		ui.lnUsername->clear();
		ui.lnUsername->insert(settings->value("username").toString());
		ui.lnPassword->clear();
		ui.lnPassword->insert(settings->value("password").toString());
	}
}

frmLogin::~frmLogin()
{
}

void frmLogin::login()
{
	/// ��ȡ�û�������û�����������Ϣ
	QString username, password;
	username = ui.lnUsername->text();
	password = ui.lnPassword->text();

	/// ����HTTP����
	QByteArray reqdata;
	reqdata.append("username=");
	reqdata.append(username);
	reqdata.append("&password=");
	reqdata.append(password);
	reqdata.append("&version=");
	reqdata.append(((LXDQApp *)qApp)->ver);
	QNetworkReply *rep = accessmanager->post(QNetworkRequest(QUrl("http://" + ((LXDQApp *)qApp)->host + "/login")), reqdata);


	/// ��ʼ��¼���ȴ�����
	ui.btnLogin->setDisabled(true);
	ui.btnJoinus->setDisabled(true);

	this->isBeggar = false;
}

void frmLogin::triallogin()
{
	/// ��ȡ�û�������û�����������Ϣ
	QString username, password;
	username = ui.lnUsername->text();
	password = ui.lnPassword->text();

	/// ��ȡ������
	HW_PROFILE_INFO hwProfInfo;
	GetCurrentHwProfile(&hwProfInfo);

	/// ����HTTP����
	QByteArray reqdata;
	reqdata.append("HWID=");
	reqdata.append(QString::fromWCharArray(hwProfInfo.szHwProfileGuid));
	reqdata.append("&version=");
	reqdata.append(((LXDQApp *)qApp)->ver);
	QNetworkReply *rep = accessmanager->post(QNetworkRequest(QUrl("http://" + ((LXDQApp *)qApp)->host + "/requesttrial")), reqdata);


	/// ��ʼ��¼���ȴ�����
	ui.btnLogin->setDisabled(true);
	ui.btnJoinus->setDisabled(true);

	this->isBeggar = true;
}

void frmLogin::on_login_recieved(QNetworkReply *rep)
{
	QIcon ico(":/LXDInjector/LXDInjector.ico");
	QString respstr = rep->readAll();
	rep->deleteLater();
	QJsonDocument respjson = QJsonDocument::fromJson(respstr.toUtf8().data());
	if (respjson.isNull())
	{
		QMessageBox message(QMessageBox::NoIcon, QString::fromWCharArray(L"����"), QString::fromWCharArray(L"����������ʧ�ܣ���������ά�������Ժ����ԣ�"));
		message.setWindowIcon(ico);
		message.exec();
		ui.btnLogin->setDisabled(false);
		ui.btnJoinus->setDisabled(false);
		return;
	}
	QJsonObject obj = respjson.object();
	QJsonValue jsonvalstatus = obj.value(QString::fromWCharArray(L"status"));
	QJsonValue jsonvalsessionkey = obj.value(QString::fromWCharArray(L"sessionkey"));
	if (jsonvalstatus.isUndefined())
	{
		QMessageBox message(QMessageBox::NoIcon, QString::fromWCharArray(L"����"), QString::fromWCharArray(L"����������ʧ�ܣ���������ά�������Ժ����ԣ�"));
		message.setWindowIcon(ico);
		message.exec();
		ui.btnLogin->setDisabled(false);
		ui.btnJoinus->setDisabled(false);
		return;
	}
	QString status = jsonvalstatus.toString();
	if (status == QString::fromWCharArray(L"success"))
	{
		if (ui.cbRemember->isChecked())
		{
			settings->setValue("username", ui.lnUsername->text());
			settings->setValue("password", ui.lnPassword->text());
		}
		QString sessionkey = jsonvalsessionkey.toString();
		// QMessageBox message(QMessageBox::NoIcon, QString::fromWCharArray(L"debug"), QString::fromWCharArray(L"result:") + status + QString::fromWCharArray(L" ") + sessionkey);
		// message.exec();
		ui.btnLogin->setDisabled(false);
		LXDQApp *lApp = (LXDQApp *)qApp;
		lApp->sessionkey = sessionkey;
		QDialog::accept();
		return;
	}
	else
	{
		if (this->isBeggar)
		{
			QString msg = QString::fromWCharArray(L"<p>�������ܾ��������������󣬿������������Ѿ����ù��ˣ���ӭ������������</p>");
			msg += QString::fromWCharArray(L"<p>Ҳ���������İ汾�����ˣ���&nbsp;<a style=\"color: #BBBBBB\" href=\"https://luoxiandu.com\">�������ǵĹ���</a>&nbsp;��ȡ���°汾��</p>");
			msg += QString::fromWCharArray(L"<p>��������������⣬��ӭ&nbsp;<a style=\"color: #BBBBBB\" href=\"https://shang.qq.com/wpa/qunwpa?idkey=c9f0a58ddf654dfee69356d744d426607f5f2ed076c09c1be78e5af2a4f893a8\">�������ɶ�QQȺ��105976356��</a>&nbsp;�����������ۡ�</p>");
			QMessageBox message(QMessageBox::NoIcon, QString::fromWCharArray(L"����"), msg);
			message.setWindowIcon(ico);
			message.exec();
		}
		else
		{
			QString msg = QString::fromWCharArray(L"<p>��¼ʧ�ܣ�����QQ�ź������Ƿ����󣬻��߰汾�Ƿ�Ϊ���£�</p>");
			msg += QString::fromWCharArray(L"<p>������������İ汾�����ˣ���&nbsp;<a style=\"color: #BBBBBB\" href=\"https://luoxiandu.com\">�������ǵĹ���</a>&nbsp;��ȡ���°汾��</p>");
			QMessageBox message(QMessageBox::NoIcon, QString::fromWCharArray(L"����"), msg);
			message.setWindowIcon(ico);
			message.exec();
		}
		ui.btnLogin->setDisabled(false);
		ui.btnJoinus->setDisabled(false);
		return;
	}
}

void frmLogin::exit()
{
	QApplication::quit();
}

void frmLogin::joinus()
{
	QIcon ico(":/LXDInjector/LXDInjector.ico");
	QString msg = QString::fromWCharArray(L"<p>ע���û���¼���ø��๦��Ŷ��</p><p>����ע�ᣬ��&nbsp;<a style=\"color: #BBBBBB\" href=\"https://shang.qq.com/wpa/qunwpa?idkey=c9f0a58ddf654dfee69356d744d426607f5f2ed076c09c1be78e5af2a4f893a8\">�������ɶ�QQȺ��105976356��</a>&nbsp;����������˷��͡��������롱�������������롣</p>");
	QMessageBox message(QMessageBox::Information, QString::fromWCharArray(L"��Ϣ"), msg);
	message.setWindowIcon(ico);
	message.exec();
}

void frmLogin::on_btnJoinus_clicked()
{
	this->joinus();
	this->triallogin();
}

void frmLogin::on_btnLogin_clicked()
{
	if (!ui.cbRemember->isChecked())
	{
		settings->clear();
	}
	this->login();
}

void frmLogin::on_btnQuit_clicked()
{
	this->exit();
}
