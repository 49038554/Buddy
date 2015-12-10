#ifndef REDISCLIENT_H
#define REDISCLIENT_H
#include <map>
#include <ctime>
#include <string>
#include <vector>

//ֻ����REDIS�����ӺͶϿ�
struct redisContext;
namespace Redis
{
	enum Result
	{
		success = 0,
		unknow = 1,
		unsvr = 2,
		nullData = 3,
	};
}

class RedisConnect
{
public:
	RedisConnect(void);
	virtual ~RedisConnect(void);
	/*
	�������ӷ�����
	*/
	bool Connect(const std::string &ip, int port);

	/*
	���Թر�����
	*/
	void Close();

	bool DelData(const std::string &key);
	//////////////////////////////////////////////////////////////////////////
	//Map����
	bool SetMapItem(const std::string &mapName, const std::string &itemKey, const std::string &itemValue);
	Redis::Result GetMapItem(const std::string &mapName, const std::string &itemKey, std::string &itemValue);
	Redis::Result GetMap(const std::string &mapName, std::map<std::string,std::string> &items);
	Redis::Result MapItemCount(const std::string &mapName, int &count);
	bool DelMapItem(const std::string &mapName, const std::string &itemKey);

	//////////////////////////////////////////////////////////////////////////
	//Key-Value����
	bool Keys(const std::string &key, std::vector<std::string> &items);
	bool Set(const std::string &key, const std::string &value);
	Redis::Result Get(const std::string &key, std::string &value);

	//////////////////////////////////////////////////////////////////////////
	//Set����
	bool AddSetItem(const std::string &setName, const std::string &itemValue);
	bool DelSetItem(const std::string &setName, const std::string &itemValue);
	Redis::Result GetSet(const std::string &setName, std::vector<std::string> &itemValues);
	bool ExistInSet(const std::string &setName, const std::string &itemValue);

protected:
	bool Ping();
	//��������������
	bool Connect();
	//��������
	bool HoldConnect();

private:
	redisContext* m_conn;
	bool m_isConnected;

	std::string m_ip;
	int m_port;

	time_t m_time;
};

#endif //REDISCLIENT_H