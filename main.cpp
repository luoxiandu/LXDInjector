#include "stdafx.h"
#include "LXDInjector.h"
#include "frmLogin.h"
#include "LXDQApp.h"
#include <QtWidgets/QApplication>
#include "dumpfile.h"

int main(int argc, char *argv[])
{
	VM_START
	QFile theme("Theme/Darcula/darcula.qss");
	theme.open(QIODevice::ReadOnly);
	LXDQApp a(argc, argv);
	frmLogin login;
	qApp->setStyleSheet(theme.readAll());
	// DeclareDumpFile()
	if (login.exec() == QDialog::Accepted)
	{
		LXDInjector w;
		w.show();
		w.setStatus(L"»¶Ó­À´µ½ÂåÏÉ¶¼£¡");
		return a.exec();
	}
	else
		return 0;
	VM_END
}
