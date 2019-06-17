#include <QtCore/QCoreApplication>
#include <QFile>
#include <QProcess>
#include <QThread>
#include <QLocalServer>
#include <QLocalSocket>
#include <QSharedMemory>
#include <QStringList>
#include <ThemidaSDK.h>

/*
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
*/

int main(int argc, char *argv[])
{
	VM_START
	STR_ENCRYPT_START
	QCoreApplication a(argc, argv);
	QLocalServer svr(&a);
	svr.listen("LXDGuardPipe");
	// QThread *chk = QThread::create(guard);
	// chk->start();
	QSharedMemory mem("DLLStringList");
	mem.create(81920);
	mem.attach();
	a.connect(&svr, &QLocalServer::newConnection, [&]() {
		QLocalSocket *sock = svr.nextPendingConnection();
		a.connect(sock, &QLocalSocket::disconnected, [&]() {
			QStringList *dlls = (QStringList *)mem.data();
			QProcess* process = new QProcess;
			process->execute("taskkill /F /IM GTA5.exe");
			QStringList::iterator i = dlls->begin();
			while (i != dlls->end())
			{
				QFile dll(*i);
				dll.open(QIODevice::Truncate);
				dll.write(QByteArray("fuck"));
				dll.close();
				while(!dll.remove());
				i++;
			}
			exit(0);
		});
	});
	a.exec();
	return 0;
	STR_ENCRYPT_END
	VM_END
}
