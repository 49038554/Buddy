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

int main(int argc, char* argv[])
{
	printf("%s\n", s_blessMe);

	Worker server;
	const char* pStrReason = server.Start();
	if (pStrReason)
	{
		server.m_log.Info("Error", "Start LBSSvr error:%s", pStrReason);
		return EXIT_FAILURE;
	}

	server.WaitStop();

	return EXIT_SUCCESS;
}