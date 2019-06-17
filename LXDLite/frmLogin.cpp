#include "frmLogin.h"

frmLogin::frmLogin(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
}

frmLogin::~frmLogin()
{
}

void frmLogin::on_btnLogin_clicked()
{
	VM_START
		int CheckVar1;
	STR_ENCRYPT_START
	ui.btnLogin->setDisabled(true);
	ui.lineEdit->setDisabled(true);
	HW_PROFILE_INFO hwProfInfo;
	CHECK_CODE_INTEGRITY(CheckVar1, 0x1C36EA9B)
	GetCurrentHwProfile(&hwProfInfo);
	((LXDQApp *)qApp)->HWID = QString::fromWCharArray(hwProfInfo.szHwProfileGuid);

	QByteArray reqdata;
	reqdata.append("passkey=");
	reqdata.append(ui.lineEdit->text());
	reqdata.append("&HWID=");
	reqdata.append(((LXDQApp *)qApp)->HWID);
	QNetworkReply *rep = accessmanager.post(QNetworkRequest(QUrl("http://" + ((LXDQApp *)qApp)->host + "/passkeylogin")), reqdata);
	STR_ENCRYPT_END
	if (CheckVar1 != 0x1C36EA9B) qApp->quit();
	STR_ENCRYPTW_START
	if (rep->error() == QNetworkReply::NoError)
	{
		connect(&accessmanager, SIGNAL(finished(QNetworkReply*)), this, SLOT(on_login_recieved(QNetworkReply*)));
	}
	else
	{
		QMessageBox message(QMessageBox::Warning, QString::fromWCharArray(L"错误"), QString::fromWCharArray(L"下载出错，服务器可能正在维护，请重试。"));
		message.exec();
		ui.btnLogin->setDisabled(false);
	}
	return;
	STR_ENCRYPTW_END
	VM_END
}

void frmLogin::on_btnExit_clicked()
{
	qApp->quit();
}

void frmLogin::on_login_recieved(QNetworkReply *rep)
{
	VM_START
	STR_ENCRYPTW_START
	QString respstr = rep->readAll();
	rep->deleteLater();
	QJsonDocument respjson = QJsonDocument::fromJson(respstr.toUtf8().data());
	if (!respjson.isNull())
	{
		QJsonObject obj = respjson.object();
		if (obj.value("status") == "success")
		{
			((LXDQApp *)qApp)->passkey = ui.lineEdit->text();
			((LXDQApp *)qApp)->remaining = obj.value("payload").toObject().value("remaining").toString();
			this->accept();
		}
		else
		{
			QMessageBox message(QMessageBox::NoIcon, QString::fromWCharArray(L"错误"), QString::fromWCharArray(L"登录失败！请检查您的卡密输入是否正确，是否始终在一台计算机上登录，也可能是您的卡密已到期！"));
			message.exec();
		}
	}
	else
	{
		QMessageBox message(QMessageBox::NoIcon, QString::fromWCharArray(L"错误"), QString::fromWCharArray(L"服务器连接失败，可能正在维护，请稍后再试！"));
		message.exec();
	}
	disconnect(&accessmanager, SIGNAL(finished(QNetworkReply*)), this, SLOT(on_login_recieved(QNetworkReply*)));
	ui.btnLogin->setDisabled(false);
	ui.lineEdit->setDisabled(false);
	return;
	STR_ENCRYPTW_END
	VM_END
}
