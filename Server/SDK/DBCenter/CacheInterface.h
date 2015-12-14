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
	//��ʼ����Ⱥ��Ϣ
	bool InitCluster(mdk::ConfigFile &cfg, mdk::int32 ThreadCount);
	//�û�tcp��������Ϣ
	bool SetUserNode(mdk::uint32 userId, mdk::int32 tcpEntryId);
	Redis::Result GetUserNode(mdk::uint32 userId, mdk::int32 &tcpEntryId);
	bool DelUserNode(mdk::uint32 userId);
	//�û���¼״̬,ͳһ��Authģ�����޸ģ�����ģ���ֹ����
	bool SetLoginState(mdk::uint32 userId, Cache::LoginState &state );
	Redis::Result GetLoginState(mdk::uint32 userId, Cache::LoginState &state );

	//////////////////////////////////////////////////////////////////////////
	//���½ӿڵ�д����ͳһ��DBCenterģ�����޸ģ�����ģ���ֹд����
	//���û��˺�
	bool BindUserName(mdk::int32 type, std::string username, mdk::uint32 uesrId);
	//��ѯ�û�Id
	Redis::Result GetUserId(mdk::int32 type, std::string username, mdk::uint32 &uesrId);
	//�û���Ϣ
	bool SetUserInfo(Cache::User &userInfo);
	Redis::Result GetUserInfo(Cache::User &userInfo);
	//С����б�
	bool SetBuddys(mdk::uint32 userId, Cache::IdList &list);
	Redis::Result GetBuddys(mdk::uint32 userId, Cache::IdList &list);
	//����Ⱥ��Ա
	bool SetGroupMember(mdk::uint32 groupId, Cache::IdList &list);
	Redis::Result GetGroupMember(mdk::uint32 groupId, Cache::IdList &list);
	//�¼�����
	bool AddEvent(mdk::uint32 userId, mdk::uint32 msgIndex, char *event, int size);
	Redis::Result GetEvents(mdk::uint32 userId, std::vector<std::string> &events);

	//�û���˿�б�
	bool SetUserFans(mdk::uint32 userId, Cache::IdList &list);
	Redis::Result GetUserFans(mdk::uint32 userId, Cache::IdList &list);
	//�û�ż���б�
	bool SetUserIdol(mdk::uint32 userId, Cache::IdList &list);
	Redis::Result GetUserIdol(mdk::uint32 userId, Cache::IdList &list);

private:
	//�����ض��㷨ȡ��Ⱥid
	RedisClient& GetNode(mdk::uint32 id);//��λ���
	RedisClient& PollingNode();//��ѯ���

private:
	typedef struct REDIS_NODE
	{
		mdk::int32	startPos;		//����id��ʼλ��
		mdk::int32	endPos;			//����id����λ��
		RedisClient	redis;
	}REDIS_NODE;
	std::vector<REDIS_NODE*>	m_cacheCluster;//���漯Ⱥ�����id,��㣩

};
