#include "Worker.h"

int main(int argc, char* argv[])
{
	Worker svr;
	const char *reason = svr.Start();
	if ( NULL != reason )
	{
		svr.m_log.Info( "Error", "��������:%s", reason );
		return 0;
	}
	svr.WaitStop();

	return 0;
}

