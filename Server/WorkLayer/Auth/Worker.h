#ifndef __WORKER_H__
#define __WORKER_H__

#include <string>
#include <vector>
#include <map>
#include "mdk_ex/ServerEx.h"
#include "frame/netserver/NetHost.h"
#include "mdk/Lock.h"
#include "mdk/Logger.h"
#include "mdk/ConfigFile.h"
#include "Protocl/cpp/Buffer.h"
#include "Protocl/cpp/Object/Auth/UserLogin.h"
#include "DBCenter/CacheInterface.h"
#include "DBCenter/Data/LoginState.h"
#include "Interface/ClusterMgr/cpp/ClusterMgr.h"

/**
 * Worker
 */
class Worker : public mdk::ServerEx
{
public:
	Worker(void);
	virtual ~Worker(void);

	virtual void OnCloseConnect(mdk::NetHost &host);
	virtual void OnMsg(mdk::NetHost& host);

	bool OnUserRegister(mdk::NetHost& host, msg::Buffer& buffer);            // 处理用户注册消息
	bool OnUserLogin(mdk::NetHost& host, msg::Buffer& buffer);               // 处理用户登陆消息
	bool OnUserLogout(mdk::NetHost& host, msg::Buffer& buffer);              // 处理用户登出消息
	bool OnUserResetPwd(mdk::NetHost& host, msg::Buffer& buffer);            // 处理用户重置密码
	bool OnUserBindingPhone(mdk::NetHost& host, msg::Buffer& buffer);        // 处理用户绑定手机号

private:
	friend int main(int argc, char* argv[]);
	bool SetLoginState( mdk::uint64 tcpEntryId, mdk::uint32 userId, ClientType::ClientType type, bool online);

private:
	mdk::Logger					m_log;
	mdk::ConfigFile				m_cfg;
	ClusterMgr					m_cluster;
	CacheInterface				m_cache;

	mdk::Mutex m_usersLock;
	std::map<mdk::uint64, std::map<mdk::uint32, Cache::LoginState> >	m_users;
};

#endif // __WORKER_H__