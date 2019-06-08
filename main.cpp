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
	// 开始运行
	frmLogin login;
	// DeclareDumpFile()
	if (login.exec() == QDialog::Accepted)
	{
		LXDInjector w;
		w.show();
		w.setStatus(L"欢迎来到洛仙都！");
		return a.exec();
	}
	else
		return 0;
	VM_END
}
