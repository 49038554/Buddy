#include "MysqlCluster.h"

static uint64 memToISmall(unsigned char* buf, int size)
{
	uint64 value = 0;
	value += buf[size - 1];

	for (int i = size - 2; 0 <= i; --i)
	{
		value <<= 8;
		value += buf[i];
	}

	return value;
}

MysqlCluster::MysqlCluster()
{
}


MysqlCluster::~MysqlCluster(void)
{
}

bool MysqlCluster::Init( mdk::ConfigFile &cfg )
{
	MySqlClient *pMysql;
	int count = cfg["mysql"]["count"];
	int i = 0;
	char item[256];
	for ( i = 0; i < count; i++ )
	{
		sprintf( item, "db%d", i + 1 );
		std::string db = (std::string)cfg["mysql"][item];
		int nodeCount = cfg[db]["count"];
		int j = 0;
		for ( j = 0; j < nodeCount; j++ )
		{
			sprintf( item, "ip%d", j + 1 );
			std::string ip = cfg[db][item];
			sprintf( item, "port%d", j + 1 );
			int port = cfg[db][item];
			sprintf( item, "user%d", j + 1 );
			std::string user = cfg[db][item];
			sprintf( item, "pwd%d", j + 1 );
			std::string pwd = cfg[db][item];
			pMysql = new MySqlClient;
			pMysql->SetConnectParam(ip.c_str(), db.c_str(), user.c_str(), pwd.c_str(), port );
			m_dbCluster[db].push_back(pMysql);
		}
	}
	return true;
}

MySqlClient* MysqlCluster::Node(const std::string& dbName, const std::string& account)
{
	if (m_dbCluster.end() == m_dbCluster.find(dbName)) return NULL;
	if (!m_dbCluster[dbName].size()) return NULL;

	uint64 virtualId = memToISmall((unsigned char*)account.c_str(), account.size() <= 8 ? account.size() : 8);
	int nodeId       = virtualId % m_dbCluster[dbName].size();
	m_dbCluster[dbName][nodeId]->Init();
	return m_dbCluster[dbName][nodeId];
}

MySqlClient* MysqlCluster::Node(const std::string& dbName, int userId)
{
	if (m_dbCluster.end() == m_dbCluster.find(dbName)) return NULL;
	if (!m_dbCluster[dbName].size()) return NULL;

	int nodeId = userId % m_dbCluster[dbName].size();
	m_dbCluster[dbName][nodeId]->Init();
	return m_dbCluster[dbName][nodeId];
}

MySqlClient* MysqlCluster::Node(const std::string& dbName)
{
	static int pos = 0;
	if (m_dbCluster.end() == m_dbCluster.find(dbName)) return NULL;
	if (!m_dbCluster[dbName].size()) return NULL;
	if ( pos > m_dbCluster[dbName].size() ) pos = 0;
	m_dbCluster[dbName][pos]->Init();
	MySqlClient *pMysql = m_dbCluster[dbName][pos]; 
	pos++;
	return pMysql;
}