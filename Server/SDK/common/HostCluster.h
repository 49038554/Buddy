#pragma once

#include "frame/netserver/NetHost.h"
#include "mdk/Lock.h"
#include <map>

class HostCluster
{
public:
	HostCluster(void);
	virtual ~HostCluster(void);

	void AddNode(int nodeId, mdk::NetHost &host);
	void DelNode(int nodeId);

	//选择结点，返回0失败，否则返回被选中结点id
	int Node( mdk::NetHost &host );
	//选择Auth结点,randNode = true随机选择一个结点
	int Node( const std::string &accout, mdk::NetHost &host );
	int Node( mdk::uint32 userId, mdk::NetHost &host );

private:
	int							m_nodeCount;//配置的结点数量
	std::map<int, mdk::NetHost>	m_hosts;//map<结点id,认证服务>
	mdk::Mutex					m_lockHostList;
};

