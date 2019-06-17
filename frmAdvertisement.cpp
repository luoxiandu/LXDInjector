#include "frmAdvertisement.h"

frmAdvertisement::frmAdvertisement(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	ui.btnNext->setDisabled(true);
	ui.btnPrevious->setDisabled(true);
	ui.webEngineView->page()->profile()->clearHttpCache(); //	Çå³ý»º´æ
	connect(ui.webEngineView, SIGNAL(loadFinished(bool)), this, SLOT(on_page_load(bool)));
	QNetworkReply *rep = accessmanager.get(QNetworkRequest(QUrl("http://" + ((LXDQApp *)qApp)->host + "/getadv")));
	if (rep->error() == QNetworkReply::NoError)
	{
		connect(&accessmanager, SIGNAL(finished(QNetworkReply*)), this, SLOT(on_data_recieved(QNetworkReply*)));
	}
	else
	{
		this->close();
	}
	return;
}

frmAdvertisement::~frmAdvertisement()
{
}

void frmAdvertisement::on_urlChanged(const QUrl &url)
{
	disconnect(ui.webEngineView, SIGNAL(loadFinished(bool)), this, SLOT(on_page_load(bool)));
	disconnect(ui.webEngineView, SIGNAL(urlChanged(const QUrl &)), this, SLOT(on_urlChanged(const QUrl &)));
	QDesktopServices::openUrl(url);
	ui.webEngineView->back();
}

void frmAdvertisement::on_page_load(bool ok)
{
	if(ok) connect(ui.webEngineView, SIGNAL(urlChanged(const QUrl &)), this, SLOT(on_urlChanged(const QUrl &)));
}

void frmAdvertisement::on_btnClose_clicked()
{
	this->close();
}

void frmAdvertisement::on_btnPrevious_clicked()
{
	i-- ;
	showcurrent();
}

void frmAdvertisement::on_btnNext_clicked()
{
	i++;
	showcurrent();
}

void frmAdvertisement::on_data_recieved(QNetworkReply *rep)
{
	QString respstr = rep->readAll();
	rep->deleteLater();
	QJsonDocument respjson = QJsonDocument::fromJson(respstr.toUtf8().data());
	advs = respjson.object();
	i = advs.begin();
	showcurrent();
	disconnect(&accessmanager, SIGNAL(finished(QNetworkReply*)), this, SLOT(on_data_recieved(QNetworkReply*)));
}

void frmAdvertisement::showcurrent()
{
	disconnect(ui.webEngineView, SIGNAL(loadFinished(bool)), this, SLOT(on_page_load(bool)));
	disconnect(ui.webEngineView, SIGNAL(urlChanged(const QUrl &)), this, SLOT(on_urlChanged(const QUrl &)));
	if (i != advs.begin() || i != (advs.end() - 1))
	{
		ui.btnNext->setDisabled(false);
		ui.btnPrevious->setDisabled(false);
	}
	if (i == advs.begin())
	{
		ui.btnPrevious->setDisabled(true);
	}
	if (i == (advs.end() - 1))
	{
		ui.btnNext->setDisabled(true);
	}
	ui.webEngineView->load(QUrl((*i).toString()));
	connect(ui.webEngineView, SIGNAL(loadFinished(bool)), this, SLOT(on_page_load(bool)));
}