#include "LXDLite.h"
#include "frmLogin.h"
#include "LXDQApp.h"
#include <QtWidgets/QApplication>
#include <QSharedMemory>

int main(int argc, char *argv[])
{
	// 加载主题
	QFile theme("Theme/Darcula/darcula.qss");
	theme.open(QIODevice::ReadOnly);
	LXDQApp a(argc, argv);
	qApp->setStyleSheet(theme.readAll());
	// 检测是否单例
	QSharedMemory mem("LXDInjector_singleton");
	if (!mem.create(1))
	{
		QIcon ico(":/LXDInjector/LXDInjector.ico");
		QMessageBox message(QMessageBox::NoIcon, QString::fromWCharArray(L"启动失败"), QString::fromWCharArray(L"本机已经有一个客户端正在运行，请检查系统托盘。"));
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
