#include "LXDQApp.h"



LXDQApp::LXDQApp(int argc, char* argv[]):QApplication(argc, argv)
{
	VM_START
	STR_ENCRYPT_START
	ver = tr("1.61"); // 版本号
	host = tr("dll.luoxiandu.com:5731"); // 服务器根地址
	STR_ENCRYPT_END
	VM_END
}


LXDQApp::~LXDQApp()
{
}
