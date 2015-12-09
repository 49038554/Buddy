#ifndef __DBCENTERCLUSTER_H__
#define __DBCENTERCLUSTER_H__

#include "DBCenter.h"
#include <map>

//�������ļ�Ⱥ��(���̰߳�ȫ)
class DBCenterCluster
{
public:
	DBCenterCluster(void);
	~DBCenterCluster(void);

	bool CheckCluster();
	void AddNode( int nodeId, const std::string &ip, int port);
	// �˺�->virtualId�� virtualId%�������
	DBCenter* Node(const std::string& strAccount);
	// userId%�������
	DBCenter* Node(int userId);

private:
	std::map<int, DBCenter*> m_nodes;
};

#endif // __DBCENTERCLUSTER_H__