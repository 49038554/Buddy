#pragma once

#include <string>
#include <vector>
#include "Protocl/cpp/base/Client.h"
#include "Protocl/cpp/Object/Operation/Cluster.h"

class ClusterMgr : public net::Client
{
public:
	ClusterMgr();
	~ClusterMgr();

	//取集群信息
	ResultCode::ResultCode GetCluster( Moudle::Moudle moudleId, msg::Cluster &data, std::string &reason );
};
