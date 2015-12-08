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

	//取集群信息
	SyncClient::ErrorCode GetCluster( Moudle::Moudle moudleId, msg::Cluster &data, std::string &reason );
};
