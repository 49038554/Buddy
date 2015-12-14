#include "HostCluster.h"
#include "common.h"

HostCluster::HostCluster(void)
{
	m_nodeCount = 0;
}


HostCluster::~HostCluster(void)
{
}

void HostCluster::SetNodeCount( int count )
{
	m_nodeCount = count;
}

void HostCluster::AddNode(int nodeId, mdk::NetHost &host)
{
	mdk::AutoLock lock(&m_lockHostList);
	m_hosts[nodeId] = host;
}

void HostCluster::DelNode(int nodeId)
{
	mdk::AutoLock lock(&m_lockHostList);
	m_hosts.erase(nodeId);
}

int HostCluster::Node( mdk::NetHost &host )
{
	mdk::AutoLock lock(&m_lockHostList);
	if ( 0 >= m_hosts.size() ) return 0;

	static mdk::int32 pos = 0;//ÂÖÑ¯Î»ÖÃ
	if ( pos >= m_hosts.size() ) pos = 0;
	std::map<int, mdk::NetHost>::iterator it = m_hosts.begin();
	int i = 0;
	for ( i = 0; i < pos; i++ ) it++;
	host = it->second;
	pos++;

	return it->first;
}

int HostCluster::Node( const std::string &accout, mdk::NetHost &host )
{
	mdk::uint64 virUserId = memtoiSmall((unsigned char*)accout.c_str(), accout.size() <= 8?accout.size():8);
	mdk::int32 nodeId = virUserId % m_nodeCount + 1;

	mdk::AutoLock lock(&m_lockHostList);
	if ( m_hosts.end() == m_hosts.find(nodeId) ) return 0;
	host = m_hosts[nodeId];

	return nodeId;
}

int HostCluster::Node( mdk::uint32 userId, mdk::NetHost &host )
{
	mdk::int32 nodeId = userId % m_nodeCount + 1;

	mdk::AutoLock lock(&m_lockHostList);
	if ( m_hosts.end() == m_hosts.find(nodeId) ) return 0;
	host = m_hosts[nodeId];

	return nodeId;
}
