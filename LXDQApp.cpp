#include "LXDQApp.h"



LXDQApp::LXDQApp(int argc, char* argv[]):QApplication(argc, argv)
{
	VM_START
	STR_ENCRYPT_START
	ver = tr("1.61"); // �汾��
	host = tr("dll.luoxiandu.com:5731"); // ����������ַ
	STR_ENCRYPT_END
	VM_END
}


LXDQApp::~LXDQApp()
{
}
