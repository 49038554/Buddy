#include "DBCenterCluster.h"

//将一个整数存储到字节流buf，按照小端字节序(低位在前，高位在后)
static void itomemSmall( unsigned char *buf, uint64 value, int size )
{
	int move = (size-1) * 8;
	int del = (size-1) * 8;
	int i = 0;
	for ( i = 0; i < size; i++ )
	{
		buf[i] = (char)((value << move) >> del);
		move -= 8;
	}
}

//将buf字节流专程整数，按照小端字节序(低位在前，高位在后)
static uint64 memtoiSmall( unsigned char *buf, int size )
{
	uint64 value = 0;
	int i = 0;
	value += buf[size - 1];
	for ( i = size - 2; 0 <= i; i-- )
	{
		value <<= 8;
		value += buf[i];
	}
	return value;
}

DBCenterCluster::DBCenterCluster(void)
{
}

DBCenterCluster::~DBCenterCluster(void)
{
	int nodeId = 1;
	for ( nodeId = 1; nodeId <= m_nodes.size(); nodeId++ )
	{
		if (m_nodes[nodeId] != NULL)
		{
			delete m_nodes[nodeId];
			m_nodes[nodeId] = NULL;
		}
	}
	m_nodes.clear();
}

bool DBCenterCluster::CheckCluster()
{
	std::map<int, DBCenter*>::iterator it;
	int i = 0;
	for ( i = 1; i <= m_nodes.size(); i++ )
	{
		if ( m_nodes.end() == m_nodes.find(i) ) return false;
	}

	return true;
}

void DBCenterCluster::AddNode( int nodeId, const std::string &ip, int port)
{
	DBCenter *pDB = new DBCenter();
	pDB->SetServerInfo(ip, port);
	m_nodes[nodeId] = pDB;
}

// 账号->virtualId， virtualId%结点数量
DBCenter* DBCenterCluster::Node(const std::string& account)
{
	uint64 virtualId = memtoiSmall((unsigned char*)account.c_str(), account.size() <= 8 ? account.size() : 8);
	int nodeId = virtualId % m_nodes.size() + 1;
	return m_nodes[nodeId];
}

// userId%结点数量
DBCenter* DBCenterCluster::Node(int userId)
{
	int nodeId = userId % m_nodes.size() + 1;
	return m_nodes[nodeId];
}
