#pragma once

#include <QDialog>
#include <QSettings>
#include <QFileDialog>
#include "ui_frmDirectoryCorrection.h"

class frmDirectoryCorrection : public QDialog
{
	Q_OBJECT

public:
	frmDirectoryCorrection(QWidget *parent = Q_NULLPTR);
	~frmDirectoryCorrection();

private:
	Ui::frmDirectoryCorrection ui;
	QSettings *GTAreg;

private slots:
	void on_btnConfirm_clicked();
	void on_btnBrowse_clicked();
};
