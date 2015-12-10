#include "RedisLock.h"
#include "../include/RedisClient.h"

#ifdef WIN32
#include <WinSock2.h>
#include <Windows.h>
#pragma comment(lib, "ws2_32.lib")
#endif

RedisClient::RedisClient(void)
{
	m_mutex = new RedisMutex;
}
RedisClient::~RedisClient(void)
{
	delete m_mutex;
	for (int i=0; i<m_idleClients.size(); i++)
	{
		delete m_idleClients[i];
	}
}

bool RedisClient::DelData(const std::string &key)
{
	RedisConnect* pConn = Alloc();
	if (!pConn) return false;
	bool ret = pConn->DelData(key);
	Free(pConn);
	return ret;
}

/////////////////////////////////////////////////////////////////////////
//Map²Ù×÷
bool RedisClient::SetMapItem(const std::string &mapName, const std::string &itemKey, const std::string &itemValue)
{
	RedisConnect* pConn = Alloc();
	if (!pConn) return false;
	bool ret = pConn->SetMapItem(mapName, itemKey, itemValue);
	Free(pConn);
	return ret;
}

Redis::Result RedisClient::GetMapItem(const std::string &mapName, const std::string &itemKey, std::string &itemValue)
{
	RedisConnect* pConn = Alloc();
	if (!pConn) return Redis::unknow;
	Redis::Result ret = pConn->GetMapItem(mapName, itemKey, itemValue);
	Free(pConn);
	return ret;
}

Redis::Result RedisClient::GetMap(const std::string &mapName, std::map<std::string,std::string> &items)
{
	RedisConnect* pConn = Alloc();
	if (!pConn) return Redis::unknow;
	Redis::Result ret = pConn->GetMap(mapName, items);
	Free(pConn);
	return ret;
}

Redis::Result RedisClient::MapItemCount(const std::string &mapName, int &count)
{
	RedisConnect* pConn = Alloc();
	if (!pConn) return Redis::unknow;
	Redis::Result ret = pConn->MapItemCount(mapName, count);
	Free(pConn);
	return ret;
}

bool RedisClient::DelMapItem(const std::string &mapName, const std::string &itemKey)
{
	RedisConnect* pConn = Alloc();
	if (!pConn) return false;
	bool ret = pConn->DelMapItem(mapName, itemKey);
	Free(pConn);
	return ret;
}

//////////////////////////////////////////////////////////////////////////
//Key-Value²Ù×÷
bool RedisClient::Keys(const std::string &key, std::vector<std::string> &items)
{
	RedisConnect* pConn = Alloc();
	if (!pConn) return false;
	bool ret = pConn->Keys(key, items);
	Free(pConn);
	return ret;
}

bool RedisClient::Set(const std::string &key, const std::string &value)
{
	RedisConnect* pConn = Alloc();
	if (!pConn) return false;
	bool ret = pConn->Set(key, value);
	Free(pConn);
	return ret;
}

Redis::Result RedisClient::Get(const std::string &key, std::string &value)
{
	RedisConnect* pConn = Alloc();
	if (!pConn) return Redis::unknow;
	Redis::Result ret = pConn->Get(key, value);
	Free(pConn);
	return ret;
}

//////////////////////////////////////////////////////////////////////////
//Set²Ù×÷
bool RedisClient::AddSetItem(const std::string &setName, const std::string &itemValue)
{
	RedisConnect* pConn = Alloc();
	if (!pConn) return false;
	bool ret = pConn->AddSetItem(setName, itemValue);
	Free(pConn);
	return ret;
}

bool RedisClient::DelSetItem(const std::string &setName, const std::string &itemValue)
{
	RedisConnect* pConn = Alloc();
	if (!pConn) return false;
	bool ret = pConn->DelSetItem(setName, itemValue);
	Free(pConn);
	return ret;
}

Redis::Result RedisClient::GetSet(const std::string &setName, std::vector<std::string> &itemValues)
{
	RedisConnect* pConn = Alloc();
	if (!pConn) return Redis::unknow;
	Redis::Result ret = pConn->GetSet(setName, itemValues);
	Free(pConn);
	return ret;
}

bool RedisClient::ExistInSet(const std::string &setName, const std::string &itemValue)
{
	RedisConnect* pConn = Alloc();
	if (!pConn) return false;
	bool ret = pConn->ExistInSet(setName, itemValue);
	Free(pConn);
	return ret;
}


void RedisClient::SetThreadCount(std::string &host, int port, int count)
{
	m_ip = host;
	m_port = port;
	for (int i=0; i<count; i++)
	{
		RedisConnect* conn = new RedisConnect();
		conn->Connect(m_ip, m_port);
		m_idleClients.push_back(conn);
	}
}

RedisConnect* RedisClient::Alloc()
{
	m_mutex->Lock();
	if (m_idleClients.empty())
	{
		RedisConnect* conn = new RedisConnect();
		conn->Connect(m_ip, m_port);
		//m_idleClients.push_back(conn);
		m_mutex->Unlock();
		return conn;
		//m_mutex->Unlock();
		//return NULL;
	}
	RedisConnect* conn = m_idleClients.back();
	m_idleClients.pop_back();
	m_mutex->Unlock();
	return conn;
}

void RedisClient::Free(RedisConnect* conn)
{
	m_mutex->Lock();
	m_idleClients.push_back(conn);
	m_mutex->Unlock();
}

bool RedisClient::InitNetwork()
{
#ifdef WIN32
	WSADATA *lpwsaData = NULL;
	WSADATA wsaData;
	if (lpwsaData == NULL)
		lpwsaData = &wsaData;

	WORD wVersionRequested = MAKEWORD(1, 1);
	__int32 nResult = WSAStartup(wVersionRequested, lpwsaData);
	if (nResult != 0) return false;
	if (LOBYTE(lpwsaData->wVersion) != 1 || HIBYTE(lpwsaData->wVersion) != 1)
	{
		WSACleanup();
		return false;
	}
#endif
	return true;
}

