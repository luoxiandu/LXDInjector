#include "LXDQApp.h"



LXDQApp::LXDQApp(int argc, char* argv[]):QApplication(argc, argv)
{
	ver = tr("1.32"); // �汾��
	host = tr("dll.luoxiandu.com:5731");
}


LXDQApp::~LXDQApp()
{
}
