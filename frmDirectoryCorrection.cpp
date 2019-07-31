#include "frmDirectoryCorrection.h"

frmDirectoryCorrection::frmDirectoryCorrection(QWidget *parent)
	: QDialog(parent)
{
	QIcon ico(":/LXDInjector/LXDInjector.ico");
	setWindowIcon(ico);
	ui.setupUi(this);
	GTAreg = new QSettings("HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\Steam App 271590", QSettings::NativeFormat);
	ui.lePath->setText(GTAreg->value("InstallLocation").toString());
}

frmDirectoryCorrection::~frmDirectoryCorrection()
{
}

void frmDirectoryCorrection::on_btnConfirm_clicked()
{
	GTAreg->setValue("InstallLocation", ui.lePath->text());
	close();
}

void frmDirectoryCorrection::on_btnBrowse_clicked()
{
	QString result = QFileDialog::getExistingDirectory(this, QString::fromWCharArray(L"请选择您安装GTA5的目录："), ui.lePath->text());
	if (!result.isEmpty()) ui.lePath->setText(result);
}
