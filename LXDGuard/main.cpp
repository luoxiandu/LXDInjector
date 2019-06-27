#include <QtCore/QCoreApplication>
#include <QFile>
#include <QProcess>
#include <QThread>
#include <QLocalServer>
#include <QLocalSocket>
#include <QSharedMemory>
#include <QStringList>
#include <ThemidaSDK.h>
#include <Windows.h>
#include <tlhelp32.h>
#include <string.h>
using namespace std;

int killTask(const QString& exe)
{
	VM_START
	//1、根据进程名称找到PID
	HANDLE hProcessSnap;
	PROCESSENTRY32 pe32;

	hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hProcessSnap == INVALID_HANDLE_VALUE)
	{
		return -1;
	}

	pe32.dwSize = sizeof(PROCESSENTRY32);

	if (!Process32First(hProcessSnap, &pe32))
	{
		CloseHandle(hProcessSnap);
		return -1;
	}

	BOOL    bRet = FALSE;
	DWORD dwPid = -1;
	while (Process32Next(hProcessSnap, &pe32))
	{
		//将WCHAR转成const char*
		int iLn = WideCharToMultiByte(CP_UTF8, 0, const_cast<LPWSTR> (pe32.szExeFile), static_cast<int>(sizeof(pe32.szExeFile)), NULL, 0, NULL, NULL);
		std::string result(iLn, 0);
		WideCharToMultiByte(CP_UTF8, 0, pe32.szExeFile, static_cast<int>(sizeof(pe32.szExeFile)), const_cast<LPSTR> (result.c_str()), iLn, NULL, NULL);
		if (0 == strcmp(exe.toStdString().c_str(), result.c_str()))
		{
			dwPid = pe32.th32ProcessID;
			bRet = TRUE;
			qDebug() << "zhaodao";
			break;
		}
	}

	CloseHandle(hProcessSnap);
	qDebug() << dwPid;
	//2、根据PID杀死进程
	HANDLE hProcess = NULL;
	//打开目标进程
	hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, dwPid);
	if (hProcess == NULL) {
		qDebug() << "Open Process fAiled ,error:" << GetLastError();
		return -1;
	}
	//结束目标进程
	DWORD ret = TerminateProcess(hProcess, 0);
	if (ret == 0) {
		qDebug() << "kill task faild,error:" << GetLastError();
		return -1;
	}

	return 0;
	VM_END
}

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
		a.connect(sock, &QLocalSocket::readyRead, [&]() {
			QString recv = sock->readAll();
		});
		a.connect(sock, &QLocalSocket::disconnected, [&]() {
			QStringList *dlls = (QStringList *)mem.data();
			QProcess* process = new QProcess;
			killTask("GTA5.exe");
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
