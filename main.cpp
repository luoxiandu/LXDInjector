#include "stdafx.h"
#include "LXDInjector.h"
#include "frmLogin.h"
#include "LXDQApp.h"
#include <QtWidgets/QApplication>
#include <QSharedMemory>
#include "dumpfile.h"

int main(int argc, char *argv[])
{
	VM_START
	// ��������
	QFile theme("Theme/Darcula/darcula.qss");
	theme.open(QIODevice::ReadOnly);
	LXDQApp a(argc, argv);
	qApp->setStyleSheet(theme.readAll());
	// ����Ƿ���
	QSharedMemory mem("LXDInjector_singleton");
	if (!mem.create(1))
	{
		QIcon ico(":/LXDInjector/LXDInjector.ico");
		QMessageBox message(QMessageBox::NoIcon, QString::fromWCharArray(L"����ʧ��"), QString::fromWCharArray(L"�����Ѿ���һ���ͻ����������У�����ϵͳ���̡�"));
		message.setWindowIcon(ico);
		return message.exec();
	}
	// ��ʼ����
	frmLogin login;
	// DeclareDumpFile()
	if (login.exec() == QDialog::Accepted)
	{
		LXDInjector w;
		w.show();
		w.setStatus(L"��ӭ�������ɶ���");
		return a.exec();
	}
	else
		return 0;
	VM_END
}
