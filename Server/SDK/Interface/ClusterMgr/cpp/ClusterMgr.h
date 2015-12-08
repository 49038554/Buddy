#pragma once

#include <string>
#include <vector>
#include "Interface/SyncClient.h"
#include "Protocl/cpp/Object/Operation/Cluster.h"

class ClusterMgr : public SyncClient
{
public:
	ClusterMgr();
	~ClusterMgr();

	//ȡ��Ⱥ��Ϣ
	SyncClient::ErrorCode GetCluster( Moudle::Moudle moudleId, msg::Cluster &data, std::string &reason );
};
