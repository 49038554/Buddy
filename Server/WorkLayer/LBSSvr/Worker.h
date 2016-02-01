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

	// 忽略不处理的消息，并记录Log
	virtual void OnConnect(mdk::NetHost &host);
	virtual void OnCloseConnect(mdk::NetHost &host);
	virtual void OnMsg(mdk::NetHost& host);

	void OnOpenSkillShop();
	void OnPutInfo();

private:
	friend int main(int argc, char* argv[]);
	bool SendNotify(mdk::uint32 recverId, unsigned char recvType, msg::Buffer *pMsg, time_t holdTime = -1, mdk::uint32 sender = 0, const std::string &senderName = "系统");
	//结点信息
	class ConnectInfo : public mdk::HostData
	{
	public:
		mdk::int32			nodeId;//结点id
		Moudle::Moudle		type;//结点类型
		NetLine::NetLine	lineType;//运营商线路
		std::string			ip;//地址
		mdk::int32			port;//端口
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