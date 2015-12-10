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

	//ѡ���㣬����0ʧ�ܣ����򷵻ر�ѡ�н��id
	int Node( mdk::NetHost &host );
	//ѡ��Auth���,randNode = true���ѡ��һ�����
	int Node( const std::string &accout, mdk::NetHost &host );
	int Node( mdk::uint32 userId, mdk::NetHost &host );

private:
	int							m_nodeCount;//���õĽ������
	std::map<int, mdk::NetHost>	m_hosts;//map<���id,��֤����>
	mdk::Mutex					m_lockHostList;
};

