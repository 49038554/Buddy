#pragma once

#include "frame/netserver/NetServer.h"
#include "mdk/Logger.h"
#include "mdk/ConfigFile.h"
#include "mdk/Lock.h"
#include "Protocl/cpp/Buffer.h"
#include "DBCenter/CacheInterface.h"
#include "Interface/ClusterMgr/cpp/ClusterMgr.h"

class Worker : public mdk::NetServer
{
	friend int main(int argc, char* argv[]);
public:
	Worker(void);
	virtual ~Worker(void);

	virtual void OnConnect(mdk::NetHost &host);
	virtual void OnCloseConnect(mdk::NetHost &host);
	virtual void OnMsg(mdk::NetHost &host);

protected:
	void OnEvent(mdk::NetHost &host, msg::Buffer &buffer);
	void OnGetEvent(mdk::NetHost &host, msg::Buffer &buffer);

private:
	bool InitId( const std::string &fileName );
	bool CreateId( mdk::uint32 &id, bool now = true );
	void SaveId( bool now );


private:
	mdk::Logger		m_log;
	mdk::ConfigFile	m_cfg;
	ClusterMgr	m_cluster;
	mdk::uint16 m_nodeId;
	mdk::uint32 m_searialNo;
	mdk::ConfigFile *m_pCfg;
	CacheInterface	m_cache;
	std::map<int, mdk::NetHost>	m_tcpEntryMap;
	mdk::Mutex					m_lockTcpEntryMap;
	typedef struct NODE_INFO
	{
		int nodeId;
	}NODE_INFO;
};
