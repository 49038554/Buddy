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

	// ���Բ��������Ϣ������¼Log
	virtual void OnMsg(mdk::NetHost& host);

	bool OnUserRegister(mdk::NetHost& host, msg::Buffer& buffer);            // �����û�ע����Ϣ
	bool OnUserLogin(mdk::NetHost& host, msg::Buffer& buffer);               // �����û���½��Ϣ
	bool OnUserLogout(mdk::NetHost& host, msg::Buffer& buffer);              // �����û��ǳ���Ϣ
	bool OnUserResetPwd(mdk::NetHost& host, msg::Buffer& buffer);            // �����û���������
	bool OnUserBindingPhone(mdk::NetHost& host, msg::Buffer& buffer);        // �����û����ֻ���

private:
	friend int main(int argc, char* argv[]);

private:
	mdk::Logger					m_log;
	mdk::ConfigFile				m_cfg;
	ClusterMgr					m_cluster;
	CacheInterface				m_cache;
};

#endif // __WORKER_H__