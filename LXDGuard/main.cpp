#include <QtCore/QCoreApplication>
#include <QProcess>
#include <QThread>
#include <QLocalServer>
#include <ThemidaSDK.h>

bool CheckAppRunningStatus(const QString &appName)
{
	VM_START
	QProcess* process = new QProcess;
	process->start("tasklist", QStringList() << "/FI" << "imagename eq " + appName);
	process->waitForFinished();
	QString outputStr = QString::fromLocal8Bit(process->readAllStandardOutput());
	if (outputStr.contains(appName)) {
		return true;
	}
	else {
		return false;
	}
	VM_END
}

int guard()
{
	VM_START
	STR_ENCRYPT_START
	while (true)
	{
		if (!CheckAppRunningStatus(QString("LXDInjector.exe")))
		{
			QProcess* process = new QProcess;
			process->execute("taskkill /F /IM GTA5.exe");
			exit(0);
		}
	}
	STR_ENCRYPT_END
	VM_END
}

int main(int argc, char *argv[])
{
	VM_START
	STR_ENCRYPT_START
	QCoreApplication a(argc, argv);
	QLocalServer svr(&a);
	svr.listen("LXDGuardPipe");
	QThread *chk = QThread::create(guard);
	chk->start();
	a.exec();
	return 0;
	STR_ENCRYPT_END
	VM_END
}
