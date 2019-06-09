#include "Deposit.h"

Deposit::Deposit(QWidget *parent)
	: QDialog(parent)
{
	QIcon ico(":/LXDInjector/LXDInjector.ico");
	setWindowIcon(ico);
	ui.setupUi(this);
}

Deposit::~Deposit()
{
}

void Deposit::on_btnClose_clicked()
{
	this->close();
}

void Deposit::on_btnGetCode_clicked()
{
	ui.btnGetCode->setDisabled(true);
	ui.lbQRCode->setText(QString::fromWCharArray(L"<h1>正在获取……</h1>"));
	QByteArray reqdata;
	reqdata.append("username=");
	reqdata.append(((LXDQApp *)qApp)->sessionkey.split("::")[0]);
	reqdata.append("&paytype=");
	reqdata.append(ui.bgPayMethods->checkedButton()->text());
	reqdata.append("&price=");
	reqdata.append(ui.leAmount->text());
	QNetworkReply *rep = accessmanager.post(QNetworkRequest(QUrl("http://" + ((LXDQApp *)qApp)->host + "/020pay_getQRcode")), reqdata);
	if (rep->error() == QNetworkReply::NoError)
	{
		connect(&accessmanager, SIGNAL(finished(QNetworkReply*)), this, SLOT(on_QRcode_got(QNetworkReply*)));
	}
	else
	{
		ui.lbQRCode->setText(QString::fromWCharArray(L"<h1>获取失败！</h1>"));
	}
	return;
}

void Deposit::on_QRcode_got(QNetworkReply *rep)
{
	QString result;
	QString respstr = rep->readAll();
	rep->deleteLater();
	ui.lbQRCode->setText(respstr);
	disconnect(&accessmanager, SIGNAL(finished(QNetworkReply*)), this, SLOT(on_QRcode_got(QNetworkReply*)));
	ui.btnGetCode->setDisabled(false);
}
