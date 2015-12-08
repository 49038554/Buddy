#include "RedisLock.h"

RedisMutex::RedisMutex()
{
#ifdef WIN32
	InitializeCriticalSection(&m_mutex);
#else
	int kind = PTHREAD_MUTEX_FAST_NP;
	pthread_mutexattr_t mutexattr;

	int nError = 0;
	if (0 != (nError = pthread_mutexattr_init(&mutexattr))) return;
	if (0 != (nError = pthread_mutexattr_settype(&mutexattr, kind))) return;
	if (0 != (nError = pthread_mutex_init(&m_mutex,  &mutexattr))) return;
	if (0 != (nError = pthread_mutexattr_destroy(&mutexattr))) return;
#endif
}

RedisMutex::~RedisMutex()
{
#ifdef WIN32
	DeleteCriticalSection(&m_mutex);
#else
	pthread_mutex_destroy(&m_mutex);
#endif
}

void RedisMutex::Lock()
{
#ifdef WIN32
	EnterCriticalSection(&m_mutex);
#else
	pthread_mutex_lock(&m_mutex);
#endif
}

void RedisMutex::Unlock()
{
#ifdef WIN32
	LeaveCriticalSection(&m_mutex);
#else
	pthread_mutex_unlock(&m_mutex);
#endif
}
