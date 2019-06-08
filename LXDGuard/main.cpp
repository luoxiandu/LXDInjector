#include <QtCore/QCoreApplication>
#include <QProcess>
#include <QThread>
#include <QLocalServer>

bool CheckAppRunningStatus(const QString &appName)
{
#ifdef Q_OS_WIN
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
#endif
}

int guard()
{
	while (true)
	{
		if (!CheckAppRunningStatus(QString("LXDInjector.exe")))
		{
			QProcess* process = new QProcess;
			process->execute("taskkill /F /IM GTA5.exe");
			exit(0);
		}
	}
}

int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);
	QLocalServer svr(&a);
	svr.listen("LXDGuardPipe");
	printf("%d\n", svr.isListening());
	printf(svr.fullServerName().toLocal8Bit());
	printf("\n");
	QThread *chk = QThread::create(guard);
	chk->start();
	a.exec();
	return 0;
}
