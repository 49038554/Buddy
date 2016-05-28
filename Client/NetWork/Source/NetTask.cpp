#include "NetTask.h"
#include <cstdio>

NetTask::NetTask(void)
{
	id = 0;
	type = 0;
	state = unsend;
	flushTime = time(NULL);
	pData = NULL;
	dataSize = 0;
}


NetTask::~NetTask(void)
{
	if ( NULL != pData )
	{
		delete[]pData;
		pData = NULL;
	}
}
