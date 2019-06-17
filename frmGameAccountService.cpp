#include "frmGameAccountService.h"

frmGameAccountService::frmGameAccountService(QWidget *parent)
	: QDialog(parent)
{
	QIcon ico(":/LXDInjector/LXDInjector.ico");
	setWindowIcon(ico);
	megalodonprice = ((LXDQApp *)qApp)->prices.value("megalodon").toDouble(5.0);
	whaleprice = ((LXDQApp *)qApp)->prices.value("whale").toDouble(2.0);
	greatwhiteprice = ((LXDQApp *)qApp)->prices.value("greatwhite").toDouble(1.0);
	bullsharkprice = ((LXDQApp *)qApp)->prices.value("bullshark").toDouble(0.5);
	tigersharkprice = ((LXDQApp *)qApp)->prices.value("tigershark").toDouble(0.2);
	redsharkprice = ((LXDQApp *)qApp)->prices.value("redshark").toDouble(0.1);
	levelprice = ((LXDQApp *)qApp)->prices.value("level").toDouble(10.0);
	unlockprice = ((LXDQApp *)qApp)->prices.value("unlock").toDouble(10.0);
	ui.setupUi(this);
	docalculate();
}

frmGameAccountService::~frmGameAccountService()
{
}

void frmGameAccountService::on_btnClose_clicked()
{
	this->close();
}

void frmGameAccountService::on_sbMegalodon_valueChanged(int i)
{
	megalodon = i;
	docalculate();
}

void frmGameAccountService::on_sbWhale_valueChanged(int i)
{
	whale = i;
	docalculate();
}

void frmGameAccountService::on_sbGreatwhite_valueChanged(int i)
{
	greatwhite = i;
	docalculate();
}

void frmGameAccountService::on_sbBullshark_valueChanged(int i)
{
	bullshark = i;
	docalculate();
}

void frmGameAccountService::on_sbTigershark_valueChanged(int i)
{
	tigershark = i;
	docalculate();
}

void frmGameAccountService::on_sbRedshark_valueChanged(int i)
{
	redshark = i;
	docalculate();
}

void frmGameAccountService::on_sbLevel_valueChanged(int i)
{
	level = i;
	docalculate();
}

void frmGameAccountService::on_cbUnlock_stateChanged(int i)
{
	if (i == 0)
	{
		unlock = false;
	}
	if (i == 2)
	{
		unlock = true;
	}
	docalculate();
}

void frmGameAccountService::docalculate()
{
	qreal total = 0;
	qint64 totalGameCoin = megalodon * 8000000 + whale * 3500000 + greatwhite * 1250000 + bullshark * 500000 + tigershark * 200000 + redshark * 100000;
	total += megalodon * megalodonprice + whale * whaleprice + greatwhite * greatwhiteprice + bullshark * bullsharkprice + tigershark * tigersharkprice + redshark * redsharkprice;
	if (level != 0) total += levelprice;
	if (unlock) total += unlockprice;
	ui.lbTotal->setText(QString::fromWCharArray(L"<strong>可获得游戏币</strong><br>$%1<br><strong>订单总价</strong><br>￥%2元").arg(totalGameCoin).arg(total));
}

void frmGameAccountService::on_btnOrder_clicked()
{
	QIcon ico(":/LXDInjector/LXDInjector.ico");
	QByteArray reqdata;
	reqdata.append("megalodon=");
	reqdata.append(QString("%1").arg(megalodon));
	reqdata.append("&whale=");
	reqdata.append(QString("%1").arg(whale));
	reqdata.append("&greatwhite=");
	reqdata.append(QString("%1").arg(greatwhite));
	reqdata.append("&bullshark=");
	reqdata.append(QString("%1").arg(bullshark));
	reqdata.append("&tigershark=");
	reqdata.append(QString("%1").arg(tigershark));
	reqdata.append("&redshark=");
	reqdata.append(QString("%1").arg(redshark));
	reqdata.append("&level=");
	reqdata.append(QString("%1").arg(level));
	reqdata.append("&unlock=");
	reqdata.append(QString("%1").arg(unlock));
	reqdata.append("&username=");
	reqdata.append(((LXDQApp *)qApp)->sessionkey.split("::")[0]);
	QNetworkReply *rep = accessmanager.post(QNetworkRequest(QUrl("http://" + ((LXDQApp *)qApp)->host + "/gameservice/order")), reqdata);
	if (rep->error() == QNetworkReply::NoError)
	{
		connect(&accessmanager, SIGNAL(finished(QNetworkReply*)), this, SLOT(on_OrderResult_got(QNetworkReply*)));
	}
	else
	{
		QMessageBox message(QMessageBox::Warning, QString::fromWCharArray(L"错误"), QString::fromWCharArray(L"下单时网络请求失败"));
		message.setWindowIcon(ico);
		message.exec();
	}
	return;
}

void frmGameAccountService::on_OrderResult_got(QNetworkReply *rep)
{
	QIcon ico(":/LXDInjector/LXDInjector.ico");
	QString result;
	QString respstr = rep->readAll();
	rep->deleteLater();
	QMessageBox message(QMessageBox::Information, QString::fromWCharArray(L"下单结果"), respstr);
	message.setWindowIcon(ico);
	message.exec();
	disconnect(&accessmanager, SIGNAL(finished(QNetworkReply*)), this, SLOT(on_OrderResult_got(QNetworkReply*)));
}
