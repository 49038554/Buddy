#ifndef __WORKER_H__
#define __WORKER_H__

#include <string>
#include <vector>
#include <map>
#include "frame/netserver/NetServer.h"
#include "frame/netserver/NetHost.h"
#include "mdk/Lock.h"
#include "mdk/Logger.h"
#include "mdk/ConfigFile.h"
#include "Protocl/cpp/Buffer.h"
#include "Protocl/cpp/Object/Auth/UserLogin.h"
#include "DBCenter/DBInterFace.h"
#include "DBCenter/CacheInterface.h"
#include "DBCenter/Data/LoginState.h"
#include "DBCenter/Data/User.h"
#include "Interface/ClusterMgr/cpp/ClusterMgr.h"

/**
 * Worker
 */
class Worker : public mdk::NetServer
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
	//////////////////////////////////////////////////////////////////////////
	//ѡ��DBCenter���
	// �˺�->virtualId�� virtualId%�������
	DBInterface* DBNode(const std::string& strAccount);
	// userId%�������
	DBInterface* DBNode(int userId);
	//////////////////////////////////////////////////////////////////////////
	//ѡ��DBCenter���End

private:
	friend int main(int argc, char* argv[]);

private:
	mdk::Logger		          m_log;
	mdk::ConfigFile           m_cfg;
	std::vector<DBInterface*> m_allDB;
	mdk::Mutex                m_lockDB;
	CacheInterface            m_cache;
	ClusterMgr                m_cluster;
};

#endif // __WORKER_H__