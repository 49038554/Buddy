#include "Main.h"
#include "Worker.h"
#include <cstdlib>
#include <cstdio>

static const char* s_blessMe =
"                   _ooOoo_                     \n"
"                  o8888888o                    \n"
"                  88\" . \"88                  \n"
"                  (| -_- |)                    \n"
"                  O\\  =  /O                   \n"
"               ____/`---\'\\____               \n"
"             .'  \\\\|     |//  `.             \n"
"            /  \\\\|||  :  |||//  \\           \n"
"           /  _||||| -:- |||||-  \\            \n"
"           |   | \\\\\\  -  /// |   |          \n"
"           | \\_|  \'\'\\---/\'\'  |   |       \n"
"           \\  .-\\__  `-`  ___/-. /           \n"
"         ___`. .\'  /--.--\\  `. . __          \n"
"      .\"\" \'<  `.___\\_<|>_/___.\'  >\'\"\". \n"
"     | | :  `- \\`.;`\\ _ /`;.`/ - ` : | |     \n"
"     \\  \\ `-.   \\_ __\\ /__ _/   .-` /  /   \n"
"======`-.____`-.___\\_____/___.-`____.-\'======\n"
"                   `=---=\'                    \n"
"^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n"
"^           ·ð×æ±£ÓÓ       ÓÀÎÞBUG            ^\n"
"^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n";

#ifdef WIN32

#pragma warning(disable: 4099)

#ifdef _DEBUG
#    pragma comment(lib, "mdk_d.lib")
#    pragma comment(lib, "hiredis_d.lib")
#else
#    pragma comment(lib, "mdk.lib")
#    pragma comment(lib, "hiredis.lib")
#endif // _DEBUG

#endif // WIN32

int main(int argc, char* argv[])
{
	printf("%s\n", s_blessMe);

	Worker server;
	const char* pStrReason = server.Start();
	if (pStrReason)
	{
		server.m_log.Info("Error", "Start AuthServer error:%s", pStrReason);
		return EXIT_FAILURE;
	}

	server.WaitStop();

	return 0;
}