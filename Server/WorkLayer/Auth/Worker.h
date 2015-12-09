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
#include "DBCenter/Data/User.h"
#include "Interface/ClusterMgr/cpp/ClusterMgr.h"

/**
 * Worker
 */
class Worker : public mdk::ServerEx
{
public:
	Worker(void);
	virtual ~Worker(void);

	// 忽略不处理的消息，并记录Log
	virtual void OnMsg(mdk::NetHost& host);

	bool OnUserRegister(mdk::NetHost& host, msg::Buffer& buffer);            // 处理用户注册消息
	bool OnUserLogin(mdk::NetHost& host, msg::Buffer& buffer);               // 处理用户登陆消息
	bool OnUserLogout(mdk::NetHost& host, msg::Buffer& buffer);              // 处理用户登出消息
	bool OnUserResetPwd(mdk::NetHost& host, msg::Buffer& buffer);            // 处理用户重置密码
	bool OnUserBindingPhone(mdk::NetHost& host, msg::Buffer& buffer);        // 处理用户绑定手机号

private:
	friend int main(int argc, char* argv[]);

private:
	mdk::Logger					m_log;
	mdk::ConfigFile				m_cfg;
	ClusterMgr					m_cluster;
	CacheInterface				m_cache;
};

#endif // __WORKER_H__