#include "NetTask.h"
#include <cstdio>

NetTask::NetTask(void)
{
	type = 0;
	pData = NULL;
	dataSize = 0;
	spaceSize = 0;
}


NetTask::~NetTask(void)
{
	if ( NULL != pData )
	{
		delete[]pData;
		pData = NULL;
	}
}
