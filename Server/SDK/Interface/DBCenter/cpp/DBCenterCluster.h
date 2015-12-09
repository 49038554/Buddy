#ifndef __DBCENTERCLUSTER_H__
#define __DBCENTERCLUSTER_H__

#include "DBCenter.h"
#include <map>

//数据中心集群类(非线程安全)
class DBCenterCluster
{
public:
	DBCenterCluster(void);
	~DBCenterCluster(void);

	bool CheckCluster();
	void AddNode( int nodeId, const std::string &ip, int port);
	// 账号->virtualId， virtualId%结点数量
	DBCenter* Node(const std::string& strAccount);
	// userId%结点数量
	DBCenter* Node(int userId);

private:
	std::map<int, DBCenter*> m_nodes;
};

#endif // __DBCENTERCLUSTER_H__