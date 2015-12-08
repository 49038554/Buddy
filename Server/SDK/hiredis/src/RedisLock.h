#ifndef MDK_LOCK_H
#define MDK_LOCK_H

#ifdef WIN32
#include <WinSock2.h>
#include <windows.h>
#else
#include <pthread.h>
#endif

class RedisMutex
{
public:
	RedisMutex();
	~RedisMutex();
	void Lock();
	void Unlock();		

private:
#ifdef WIN32
	CRITICAL_SECTION m_mutex;
#else
	pthread_mutex_t m_mutex;
#endif

};

#endif//MDK_LOCK_H
