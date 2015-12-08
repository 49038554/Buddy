#include "CacheInterface.h"
#include "mdk/mapi.h"
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include "Protocl/cpp/Protocl.h"

//将一个整数存储到字节流buf，按照小端字节序(低位在前，高位在后)
static void itomemSmall( unsigned char *buf, mdk::uint64 value, mdk::int32 size )
{
	mdk::int32 move = (size-1) * 8;
	mdk::int32 del = (size-1) * 8;
	mdk::int32 i = 0;
	for ( i = 0; i < size; i++ )
	{
		buf[i] = (char)((value << move) >> del);
		move -= 8;
	}
}

//将buf字节流专程整数，按照小端字节序(低位在前，高位在后)
static mdk::uint64 memtoiSmall( unsigned char *buf, mdk::int32 size )
{
	mdk::uint64 value = 0;
	mdk::int32 i = 0;
	value += buf[size - 1];
	for ( i = size - 2; 0 <= i; i-- )
	{
		value <<= 8;
		value += buf[i];
	}
	return value;
}

CacheInterface::CacheInterface()
{

}

CacheInterface::~CacheInterface()
{

}

//初始化
bool CacheInterface::InitCluster( mdk::ConfigFile &cfg, mdk::int32 ThreadCount )
{
	REDIS_NODE *pRedis;
	mdk::int32 count = cfg["Cache"]["node count"];
	mdk::int32 i = 0;
	char key[256];
	std::string ip;
	mdk::int32 port;
	for ( i = 0; i < count; i++ )
	{
		pRedis = new REDIS_NODE;
		sprintf( key, "node%d start pos", i );
		pRedis->startPos = cfg["Cache"][key];
		sprintf( key, "node%d end pos", i );
		pRedis->endPos = cfg["Cache"][key];
		sprintf( key, "node%d ip", i );
		ip = (std::string)cfg["Cache"][key];
		sprintf( key, "node%d port", i );
		port = cfg["Cache"][key];
		pRedis->redis.SetThreadCount(ip, port, ThreadCount);
		m_cacheCluster.push_back(pRedis);
	}
	for ( i = 0; i < 256; i++ ) GetNode( i );//确保所有id都有配置管理结点，否则强行崩溃

	return true;
}

RedisClient& CacheInterface::GetNode(mdk::uint32 id)
{
	mdk::int32 vid = id % 256;
	std::vector<REDIS_NODE*>::iterator it = m_cacheCluster.begin();
	REDIS_NODE *pNode;
	for ( ; it != m_cacheCluster.end(); it++ )
	{
		pNode = *it;
		if ( pNode->startPos <= vid && vid <= pNode->endPos ) 
		{
			return pNode->redis;
		}
	}

	mdk::mdk_assert(false);//不可能找不到管理结点，除非集群配置不对，强行崩溃
	RedisClient client;
	return client;
}

bool CacheInterface::SetUserNode(mdk::uint32 userId, mdk::int32 tcpEntryId)
{
	RedisClient &node = GetNode(userId);
	char key[256];
	sprintf( key, "User%u", userId );
	char value[256];
	sprintf( value, "%d", tcpEntryId );
	return node.SetMapItem(key, "tcpEntryId", value);
}

Redis::Result CacheInterface::GetUserNode(mdk::uint32 userId, mdk::int32 &tcpEntryId)
{
	RedisClient &node = GetNode(userId);
	char key[256];
	sprintf( key, "User%u", userId );
	std::string value;
	Redis::Result ret = node.GetMapItem(key, "tcpEntryId", value);
	if ( Redis::success != ret ) return ret;
	sscanf( value.c_str(), "%d", &tcpEntryId );
	if ( -1 == tcpEntryId ) return Redis::nullData;

	return ret;
}

bool CacheInterface::DelUserNode(mdk::uint32 userId)
{
	return SetUserNode(userId, -1);
}

//用户登录状态
bool CacheInterface::SetLoginState(mdk::uint32 userId, Cache::LoginState &state )
{
	RedisClient &node = GetNode(userId);
	char key[256];
	sprintf( key, "User%u", userId );
	std::string data;
	if ( !state.Build() ) return false;
	data.assign((char*)state, state.Size());
	return node.SetMapItem(key, "loginState", data);
}

Redis::Result CacheInterface::GetLoginState(mdk::uint32 userId, Cache::LoginState &state )
{
	RedisClient &node = GetNode(userId);
	char key[256];
	sprintf( key, "User%u", userId );
	std::string data;
	Redis::Result ret = node.GetMapItem(key, "loginState", data);
	if ( Redis::success != ret ) return ret;
	state.Clear();
	memcpy(state, data.c_str(),data.size());
	if ( !state.Parse() ) return Redis::nullData;

	return ret;
}

//绑定用户账号
bool CacheInterface::BindUserName(mdk::int32 type, std::string username, mdk::uint32 uesrId)
{
	mdk::uint64 virtualId = memtoiSmall((unsigned char*)username.c_str(), username.size() <= 8?username.size():8);
	RedisClient &node = GetNode(virtualId);
	char key[256];
	sprintf( key, "AccountType_%d", type );
	char value[256];
	sprintf( value, "%u", uesrId );

	return node.SetMapItem(key, username, value);
}

//查询用户Id
Redis::Result CacheInterface::GetUserId(mdk::int32 type, std::string username, mdk::uint32 &uesrId)
{
	mdk::uint64 virtualId = memtoiSmall((unsigned char*)username.c_str(), username.size() <= 8?username.size():8);
	RedisClient &node = GetNode(virtualId);
	std::string value;
	char key[256];
	sprintf( key, "AccountType_%d", type );
	Redis::Result ret = node.GetMapItem(key, username, value);
	if (Redis::success != ret ) return ret;
	sscanf( value.c_str(), "%u", &uesrId );

	return ret;
}

//用户信息
bool CacheInterface::SetUserInfo(Cache::User &userInfo)
{
	RedisClient &node = GetNode(userInfo.id);
	char key[256];
	sprintf( key, "User%u", userInfo.id );
	std::string data;
	if ( !userInfo.Build() ) return false;
	data.assign((char*)userInfo, userInfo.Size());
	return node.SetMapItem(key, "info", data);
}

Redis::Result CacheInterface::GetUserInfo(Cache::User &userInfo)
{
	RedisClient &node = GetNode(userInfo.id);
	char key[256];
	sprintf( key, "User%u", userInfo.id );
	std::string data;
	Redis::Result ret = node.GetMapItem(key, "info", data);
	if ( Redis::success != ret ) return ret;
	userInfo.Clear();
	memcpy(userInfo, data.c_str(),data.size());
	if ( !userInfo.Parse() ) return Redis::nullData;

	return ret;
}

RedisClient& CacheInterface::PollingNode()
{
	static int pos = -1;
	pos++;
	if ( pos >= m_cacheCluster.size() ) pos = 0;
	return m_cacheCluster[pos]->redis;
}

//用户粉丝列表
bool CacheInterface::SetUserFans(mdk::uint32 userId, Cache::IdList &list)
{
	RedisClient &node = GetNode(userId);
	char key[256];
	sprintf( key, "User%u", userId );
	std::string data;
	if ( !list.Build() ) return false;
	data.assign((char*)list, list.Size());
	return node.SetMapItem(key, "fans", data);

	return true;
}

Redis::Result CacheInterface::GetUserFans(mdk::uint32 userId, Cache::IdList &list)
{
	RedisClient &node = GetNode(userId);
	char key[256];
	sprintf( key, "User%u", userId );
	std::string data;
	Redis::Result ret = node.GetMapItem(key, "fans", data);
	if ( Redis::success != ret ) return ret;
	list.Clear();
	memcpy(list, data.c_str(), data.size());
	if ( !list.Parse() ) return Redis::nullData;

	return ret;
}

//用户偶像列表
bool CacheInterface::SetUserIdol(mdk::uint32 userId, Cache::IdList &list)
{
	RedisClient &node = GetNode(userId);
	char key[256];
	sprintf( key, "User%u", userId );
	std::string data;
	if ( !list.Build() ) return false;
	data.assign((char*)list, list.Size());
	return node.SetMapItem(key, "idol", data);

	return true;
}

Redis::Result CacheInterface::GetUserIdol(mdk::uint32 userId, Cache::IdList &list)
{
	RedisClient &node = GetNode(userId);
	char key[256];
	sprintf( key, "User%u", userId );
	std::string data;
	Redis::Result ret = node.GetMapItem(key, "idol", data);
	if ( Redis::success != ret ) return ret;
	list.Clear();
	memcpy(list, data.c_str(), data.size());
	if ( !list.Parse() ) return Redis::nullData;

	return ret;
}
