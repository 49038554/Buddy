#ifndef REDISCLIENTPOOL_H
#define REDISCLIENTPOOL_H
#include <map>
#include <string>
#include <vector>
#include "RedisConnect.h"

//RedisClient 只封装最基本的REDIS操作, 不涉及业务
class RedisMutex;
class RedisConnect;
class RedisClient
{
public:
	RedisClient(void);
	virtual ~RedisClient(void);

	static bool InitNetwork();
	void SetThreadCount	(std::string &host, int port, int count);

	bool DelData(const std::string &key);
	//////////////////////////////////////////////////////////////////////////
	//Map操作
	bool SetMapItem(const std::string &mapName, const std::string &itemKey, const std::string &itemValue);
	Redis::Result GetMapItem(const std::string &mapName, const std::string &itemKey, std::string &itemValue);
	Redis::Result GetMap(const std::string &mapName, std::map<std::string,std::string> &items);
	Redis::Result MapItemCount(const std::string &mapName, int &count);
	bool DelMapItem(const std::string &mapName, const std::string &itemKey);

	//////////////////////////////////////////////////////////////////////////
	//Key-Value操作
	bool Keys(const std::string &key, std::vector<std::string> &items);
	bool Set(const std::string &key, const std::string &value);
	Redis::Result Get(const std::string &key, std::string &value);

	//////////////////////////////////////////////////////////////////////////
	//Set操作
	bool AddSetItem(const std::string &setName, const std::string &itemValue);
	bool DelSetItem(const std::string &setName, const std::string &itemValue);
	Redis::Result GetSet(const std::string &setName, std::vector<std::string> &itemValues);
	bool ExistInSet(const std::string &setName, const std::string &itemValue);


private:
	RedisConnect* Alloc();
	void Free(RedisConnect* conn);

private:
	std::string m_ip;
	int m_port;

	RedisMutex *m_mutex;
	std::vector<RedisConnect*> m_idleClients;
};

#endif