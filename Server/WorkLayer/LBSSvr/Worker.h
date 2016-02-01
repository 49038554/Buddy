#ifndef __WORKER_H__
#define __WORKER_H__

#include <string>
#include <vector>
#include <map>
#include "mdk_ex/ServerEx.h"
#include "frame/netserver/NetHost.h"
#include "frame/netserver/HostData.h"
#include "mdk/Lock.h"
#include "mdk/Logger.h"
#include "mdk/ConfigFile.h"
#include "Protocl/cpp/Buffer.h"
#include "DBCenter/CacheInterface.h"
#include "Interface/ClusterMgr/cpp/ClusterMgr.h"
#include "common/HostCluster.h"

/**
 * Worker
 */
class Worker : public mdk::ServerEx
{
public:
	Worker(void);
	virtual ~Worker(void);

	// ���Բ��������Ϣ������¼Log
	virtual void OnConnect(mdk::NetHost &host);
	virtual void OnCloseConnect(mdk::NetHost &host);
	virtual void OnMsg(mdk::NetHost& host);

	void OnOpenSkillShop();
	void OnPutInfo();

private:
	friend int main(int argc, char* argv[]);
	bool SendNotify(mdk::uint32 recverId, unsigned char recvType, msg::Buffer *pMsg, time_t holdTime = -1, mdk::uint32 sender = 0, const std::string &senderName = "ϵͳ");
	//�����Ϣ
	class ConnectInfo : public mdk::HostData
	{
	public:
		mdk::int32			nodeId;//���id
		Moudle::Moudle		type;//�������
		NetLine::NetLine	lineType;//��Ӫ����·
		std::string			ip;//��ַ
		mdk::int32			port;//�˿�
	public:
		ConnectInfo(void){}
		virtual ~ConnectInfo(){}
	};

private:
	mdk::Logger					m_log;
	mdk::ConfigFile				m_cfg;
	ClusterMgr					m_cluster;
	CacheInterface				m_cache;
	HostCluster					m_notifyCluster;
};

#endif // __WORKER_H__