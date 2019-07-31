#include "stdafx.h"
#include "frmLogin.h"
#include "LXDQApp.h"

frmLogin::frmLogin(QWidget *parent)
	: QDialog(parent)
{
	QIcon ico(":/LXDInjector/LXDInjector.ico");
	this->setWindowFlag(Qt::MSWindowsFixedSizeDialogHint, true);
	// 启动守护进程
	if (!QProcess::startDetached("LXDGuard.exe"))
	{
		QMessageBox message(QMessageBox::NoIcon, QString::fromWCharArray(L"错误"), QString::fromWCharArray(L"重要服务启动失败#1，请检查本软件是否遭到篡改，或关闭任何防病毒程序。"));
		message.setWindowIcon(ico);
		message.exec();
		exit();
	}
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
	/// 获取用户输入的用户名和密码信息
	QString username, password;
	username = ui.lnUsername->text();
	password = ui.lnPassword->text();

	/// 构造HTTP请求
	QByteArray reqdata;
	reqdata.append("username=");
	reqdata.append(username);
	reqdata.append("&password=");
	reqdata.append(password);
	reqdata.append("&version=");
	reqdata.append(((LXDQApp *)qApp)->ver);
	QNetworkReply *rep = accessmanager->post(QNetworkRequest(QUrl("http://" + ((LXDQApp *)qApp)->host + "/login")), reqdata);


	/// 开始登录，等待返回
	ui.btnLogin->setDisabled(true);
	ui.btnJoinus->setDisabled(true);

	this->isBeggar = false;
}

void frmLogin::triallogin()
{
	VM_START
	int CheckVar1;
	/// 获取用户输入的用户名和密码信息
	QString username, password;
	username = ui.lnUsername->text();
	password = ui.lnPassword->text();

	CHECK_CODE_INTEGRITY(CheckVar1, 0x6668A5F9)
	/// 获取机器码
	HW_PROFILE_INFO hwProfInfo;
	GetCurrentHwProfile(&hwProfInfo);
	/// 暗桩
	if (CheckVar1 != 0x6668A5F9)
	{
		STR_ENCRYPT_START
		((LXDQApp *)qApp)->ver = "cracked";
		STR_ENCRYPT_END
	}

	/// 构造HTTP请求
	QByteArray reqdata;
	reqdata.append("HWID=");
	reqdata.append(QString::fromWCharArray(hwProfInfo.szHwProfileGuid));
	reqdata.append("&version=");
	reqdata.append(((LXDQApp *)qApp)->ver);
	QNetworkReply *rep = accessmanager->post(QNetworkRequest(QUrl("http://" + ((LXDQApp *)qApp)->host + "/requesttrial")), reqdata);


	/// 开始登录，等待返回
	ui.btnLogin->setDisabled(true);
	ui.btnJoinus->setDisabled(true);

	this->isBeggar = true;
	VM_END
}

void frmLogin::on_login_recieved(QNetworkReply *rep)
{
	VM_START
	QIcon ico(":/LXDInjector/LXDInjector.ico");
	QString respstr = rep->readAll();
	rep->deleteLater();
	QJsonDocument respjson = QJsonDocument::fromJson(respstr.toUtf8().data());
	if (respjson.isNull())
	{
		QMessageBox message(QMessageBox::NoIcon, QString::fromWCharArray(L"错误"), QString::fromWCharArray(L"服务器连接失败，可能正在维护，请稍后再试！"));
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
		QMessageBox message(QMessageBox::NoIcon, QString::fromWCharArray(L"错误"), QString::fromWCharArray(L"服务器连接失败，可能正在维护，请稍后再试！"));
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
	else if (status == QString::fromWCharArray(L"banned"))
	{
		STR_ENCRYPTW_START
		QString msg = QString::fromWCharArray(L"<p>在您上次使用客户端的过程中检测到恶性破解，已禁止您使用洛仙都软件。</p>");
		msg += QString::fromWCharArray(L"<p>如果您喜欢我们提供的外挂请考虑&nbsp;<a style=\"color: #BBBBBB\" href=\"https://shang.qq.com/wpa/qunwpa?idkey=c9f0a58ddf654dfee69356d744d426607f5f2ed076c09c1be78e5af2a4f893a8\">加入洛仙都QQ群（105976356）</a>&nbsp;永久使用。</p>");
		QMessageBox message(QMessageBox::NoIcon, QString::fromWCharArray(L"警告"), msg);
		STR_ENCRYPTW_END
		message.setWindowIcon(ico);
		message.exec();
	}
	else
	{
		if (this->isBeggar)
		{
			STR_ENCRYPTW_START
			QString msg = QString::fromWCharArray(L"<p>服务器拒绝了您的试用请求，可能是您今天已经试用过了，欢迎您明天再来！</p>");
			msg += QString::fromWCharArray(L"<p>也可能是您的版本过期了，请&nbsp;<a style=\"color: #BBBBBB\" href=\"https://luoxiandu.com\">访问我们的官网</a>&nbsp;获取最新版本！</p>");
			msg += QString::fromWCharArray(L"<p>如果您有其它问题，欢迎&nbsp;<a style=\"color: #BBBBBB\" href=\"https://shang.qq.com/wpa/qunwpa?idkey=c9f0a58ddf654dfee69356d744d426607f5f2ed076c09c1be78e5af2a4f893a8\">加入洛仙都QQ群（105976356）</a>&nbsp;来和我们讨论。</p>");
			QMessageBox message(QMessageBox::NoIcon, QString::fromWCharArray(L"错误"), msg);
			STR_ENCRYPTW_END
			message.setWindowIcon(ico);
			message.exec();
		}
		else
		{
			STR_ENCRYPTW_START
			QString msg = QString::fromWCharArray(L"<p>登录失败，请检查QQ号和密码是否有误，或者版本是否为最新！</p>");
			msg += QString::fromWCharArray(L"<p>如果您怀疑您的版本过期了，请&nbsp;<a style=\"color: #BBBBBB\" href=\"https://luoxiandu.com\">访问我们的官网</a>&nbsp;获取最新版本。</p>");
			QMessageBox message(QMessageBox::NoIcon, QString::fromWCharArray(L"错误"), msg);
			STR_ENCRYPTW_END
			message.setWindowIcon(ico);
			message.exec();
		}
		ui.btnLogin->setDisabled(false);
		ui.btnJoinus->setDisabled(false);
		return;
	}
	VM_END
}

void frmLogin::exit()
{
	QApplication::quit();
}

void frmLogin::joinus()
{
	VM_START
	QIcon ico(":/LXDInjector/LXDInjector.ico");
	STR_ENCRYPTW_START
	QString msg = QString::fromWCharArray(L"<p>注册用户登录会获得更多功能哦！</p><p>如需注册，请&nbsp;<a style=\"color: #BBBBBB\" href=\"https://shang.qq.com/wpa/qunwpa?idkey=c9f0a58ddf654dfee69356d744d426607f5f2ed076c09c1be78e5af2a4f893a8\">加入洛仙都QQ群（105976356）</a>&nbsp;，并向机器人发送“设置密码”来设置您的密码。</p>");
	QMessageBox message(QMessageBox::Information, QString::fromWCharArray(L"信息"), msg);
	STR_ENCRYPTW_END
	message.setWindowIcon(ico);
	message.exec();
	VM_END
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
	QLocalSocket sock;
	sock.connectToServer("LXDGuardPipe");
	this->exit();
}
