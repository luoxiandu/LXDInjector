#include "LXDLite.h"
#include "frmLogin.h"
#include "LXDQApp.h"
#include <QtWidgets/QApplication>
#include <QSharedMemory>

int main(int argc, char *argv[])
{
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
	frmLogin l;
	if (l.exec() == QDialog::Accepted)
	{
		LXDLite w;
		w.show();
		return a.exec();
	}
	else
		return 0;
}
