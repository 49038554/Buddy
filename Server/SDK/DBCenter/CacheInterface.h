#pragma once

#include "RedisClient.h"
#include "mdk/FixLengthInt.h"
#include <vector>
#include "mdk/ConfigFile.h"

#include "Data/LoginState.h"
#include "Data/User.h"
#include "Data/IdList.h"

class CacheInterface
{
public:
	CacheInterface();
	virtual ~CacheInterface();
	//初始化集群信息
	bool InitCluster(mdk::ConfigFile &cfg, mdk::int32 ThreadCount);
	//用户tcp接入结点信息
	bool SetUserNode(mdk::uint32 userId, mdk::int32 tcpEntryId);
	Redis::Result GetUserNode(mdk::uint32 userId, mdk::int32 &tcpEntryId);
	bool DelUserNode(mdk::uint32 userId);
	//用户登录状态,统一在Auth模块中修改，其它模块禁止操作
	bool SetLoginState(mdk::uint32 userId, Cache::LoginState &state );
	Redis::Result GetLoginState(mdk::uint32 userId, Cache::LoginState &state );

	//////////////////////////////////////////////////////////////////////////
	//以下接口的写操作统一在DBCenter模块中修改，其它模块禁止写操作
	//绑定用户账号
	bool BindUserName(mdk::int32 type, std::string username, mdk::uint32 uesrId);
	//查询用户Id
	Redis::Result GetUserId(mdk::int32 type, std::string username, mdk::uint32 &uesrId);
	//用户信息
	bool SetUserInfo(Cache::User &userInfo);
	Redis::Result GetUserInfo(Cache::User &userInfo);
	//小伙伴列表
	bool SetBuddys(mdk::uint32 userId, Cache::IdList &list);
	Redis::Result GetBuddys(mdk::uint32 userId, Cache::IdList &list);
	//设置群成员
	bool SetGroupMember(mdk::uint32 groupId, Cache::IdList &list);
	Redis::Result GetGroupMember(mdk::uint32 groupId, Cache::IdList &list);
	//事件缓存
	bool AddEvent(mdk::uint32 userId, mdk::uint32 msgIndex, char *event, int size);
	Redis::Result GetEvents(mdk::uint32 userId, std::vector<std::string> &events);

	//用户粉丝列表
	bool SetUserFans(mdk::uint32 userId, Cache::IdList &list);
	Redis::Result GetUserFans(mdk::uint32 userId, Cache::IdList &list);
	//用户偶像列表
	bool SetUserIdol(mdk::uint32 userId, Cache::IdList &list);
	Redis::Result GetUserIdol(mdk::uint32 userId, Cache::IdList &list);

private:
	//按照特定算法取集群id
	RedisClient& GetNode(mdk::uint32 id);//定位结点
	RedisClient& PollingNode();//轮询结点

private:
	typedef struct REDIS_NODE
	{
		mdk::int32	startPos;		//管理id开始位置
		mdk::int32	endPos;			//管理id结束位置
		RedisClient	redis;
	}REDIS_NODE;
	std::vector<REDIS_NODE*>	m_cacheCluster;//缓存集群（结点id,结点）

};
