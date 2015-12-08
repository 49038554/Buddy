#include "./redis/hiredis.h"
#include "../include/RedisConnect.h"
#include <cstring>

RedisConnect::RedisConnect()
{
	m_conn = NULL;
	m_isConnected = false;
	m_ip = "";
	m_port = 0;
	m_time = time_t(NULL);
}

RedisConnect::~RedisConnect()
{
	Close();
}

bool RedisConnect::HoldConnect()
{
	if (!Connect()) return false;

	time_t curr = time(NULL);
	if (curr - m_time > 10) 
	{
		m_time = curr;
		if (!Ping())
		{
			if (!Connect()) return false;
		}
	}

	return true;
}

//////////////////////////////////////////////////////////////////////////
//Map操作
bool RedisConnect::SetMapItem(const std::string &mapName, const std::string &itemKey, const std::string &itemValue)
{
	if (!HoldConnect()) return false;
	redisReply* reply = (redisReply *)redisCommand(m_conn,"HSET %s %s %b", mapName.c_str(), itemKey.c_str(), itemValue.data(), itemValue.size());
	if (!reply) 
	{
		Ping();
		return false;
	}
	freeReplyObject(reply);
	return true;
}

Redis::Result RedisConnect::GetMapItem(const std::string &mapName, const std::string &itemKey, std::string &itemValue)
{
	if (!HoldConnect()) return Redis::unsvr;
	redisReply* reply = (redisReply *)redisCommand(m_conn,"HGET %s %s", mapName.c_str(), itemKey.c_str());
	if (!reply) 
	{
		Ping();
		return Redis::unsvr;
	}
	if (REDIS_REPLY_NIL == reply->type)
	{
		freeReplyObject(reply);
		return Redis::nullData;
	}

	itemValue.assign(reply->str, reply->len);
	freeReplyObject(reply);
	return Redis::success;
}

Redis::Result RedisConnect::GetMap(const std::string &mapName, std::map<std::string,std::string> &items)
{
	if (!HoldConnect()) return Redis::unsvr;
	redisReply* reply = (redisReply *)redisCommand(m_conn,"HGETALL %s", mapName.c_str());
	if (!reply) 
	{
		Ping();
		return Redis::unsvr;
	}
	if (REDIS_REPLY_ARRAY == reply->type && 0 == reply->elements)
	{
		freeReplyObject(reply);
		return Redis::nullData;
	}
	else
	{
		for (int i=0; i<reply->elements; i+=2)
		{
			std::string value;
			value.assign(reply->element[i+1]->str, reply->element[i+1]->len);
			items.insert(std::make_pair(reply->element[i]->str, value));
		}
		freeReplyObject(reply);
		return Redis::success;
	}
}

bool RedisConnect::DelMapItem(const std::string &mapName, const std::string &itemKey)
{
	if (!HoldConnect()) return false;
	redisReply* reply = (redisReply *)redisCommand(m_conn,"HDEL %s %s", mapName.c_str(), itemKey.c_str());
	if (!reply) 
	{
		Ping();
		return false;
	}

	freeReplyObject(reply);
	return true;
}


//////////////////////////////////////////////////////////////////////////
//Key-Value操作
bool RedisConnect::Keys(const std::string &key, std::vector<std::string> &items)
{
	items.clear();
	if (!HoldConnect()) return false;
	redisReply* reply = (redisReply *)redisCommand(m_conn,"keys %s*", key.c_str());
	if (!reply) 
	{
		Ping();
		return false;
	}
	if (REDIS_REPLY_NIL == reply->type)
	{
		freeReplyObject(reply);
		return false;
	}
	else
	{
		int i = 0;
		for ( i = 0; i <  reply->elements; i++ )
		{
			items.push_back(reply->element[i]->str);
		}
		freeReplyObject(reply);
		return true;
	}
}

bool RedisConnect::Set(const std::string &key, const std::string &value)
{
	if (!HoldConnect()) return false;
	redisReply* reply = (redisReply *)redisCommand(m_conn,"SET %s %b", key.c_str(), value.data(), value.size());
	if (!reply) 
	{
		Ping();
		return false;
	}

	freeReplyObject(reply);
	return true;
}

Redis::Result RedisConnect::Get(const std::string &key, std::string &value)
{
	if (!HoldConnect()) return Redis::unsvr;
	redisReply* reply = (redisReply *)redisCommand(m_conn,"GET %s", key.c_str());
	if (!reply) 
	{
		Ping();
		return Redis::unsvr;
	}
	if (REDIS_REPLY_NIL == reply->type)
	{
		freeReplyObject(reply);
		return Redis::nullData;
	}
	else
	{
		value.assign(reply->str, reply->len);
		freeReplyObject(reply);
		return Redis::success;
	}
}

//////////////////////////////////////////////////////////////////////////
//Set操作
bool RedisConnect::AddSetItem(const std::string &setName, const std::string &itemValue)
{
	if (!HoldConnect()) return false;
	redisReply* reply = (redisReply *)redisCommand(m_conn,"SADD %s %b", setName.c_str(), itemValue.data(), itemValue.size());
	if (!reply) 
	{
		Ping();
		return false;
	}

	freeReplyObject(reply);
	return true;
}

bool RedisConnect::DelSetItem(const std::string &setName, const std::string &itemValue)
{	
	if (!HoldConnect()) return false;
	redisReply* reply = (redisReply *)redisCommand(m_conn,"SREM %s %b", setName.c_str(), itemValue.data(), itemValue.size());
	if (!reply) 
	{
		Ping();
		return false;
	}

	freeReplyObject(reply);
	return true;
}

Redis::Result RedisConnect::GetSet(const std::string &setName, std::vector<std::string> &itemValues)
{
	if (!HoldConnect()) return Redis::unsvr;
	redisReply* reply = (redisReply *)redisCommand(m_conn,"SMEMBERS %s", setName.c_str());
	if (!reply) 
	{
		Ping();
		return Redis::unsvr;
	}
	if (REDIS_REPLY_ARRAY == reply->type && 0 == reply->elements)
	{
		freeReplyObject(reply);
		return Redis::nullData;
	}
	itemValues.clear();
	for (int i=0; i<reply->elements; i++)
	{
		std::string value;
		value.assign(reply->element[i]->str, reply->element[i]->len);
		itemValues.push_back(value);
	}
	freeReplyObject(reply);
	return Redis::success;
}

bool RedisConnect::ExistInSet(const std::string &setName, const std::string &itemValue)
{
	if (!HoldConnect()) return false;
	redisReply* reply = (redisReply *)redisCommand(m_conn,"SISMEMBER %s %s", setName.c_str(), itemValue.c_str());
	if (!reply)
	{
		Ping();
		return false;
	}
	if (REDIS_REPLY_INTEGER == reply->type && 0 == reply->integer)
	{
		freeReplyObject(reply);
		return false;
	}
	else
	{
		freeReplyObject(reply);
		return true;
	}
}


bool RedisConnect::Ping()
{
	redisReply* reply = (redisReply *)redisCommand(m_conn,"PING");
	if (!reply) 
	{
		Close();
		return false;
	}
	freeReplyObject(reply);
	return true;
}

/*
无脑连接服务器
*/
bool RedisConnect::Connect(const std::string &ip, int port)
{
	m_ip = ip;
	m_port = port;
	return Connect();
}

/*
无脑连接服务器
*/
#include <cassert>
bool RedisConnect::Connect()
{
	if (m_isConnected) return true;
	m_isConnected = true;

	struct timeval timeout = { 1, 500000 };
	m_conn = redisConnectWithTimeout(m_ip.c_str(), m_port, timeout);
	if (m_conn->err)
	{
		Close();
		return false;
	}

	return true;
}

/*
无脑关闭连接
*/
void RedisConnect::Close()
{
	if (!m_isConnected) return;

	redisFree(m_conn);
	m_conn = NULL;
	m_isConnected = false;
}
