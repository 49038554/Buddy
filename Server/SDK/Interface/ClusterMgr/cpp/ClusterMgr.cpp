#include "ClusterMgr.h"
#include "Protocl/cpp/Object/Operation/GetCluster.h"

ClusterMgr::ClusterMgr()
{
}

ClusterMgr::~ClusterMgr()
{
}

//取集群信息
ResultCode::ResultCode ClusterMgr::GetCluster( Moudle::Moudle moudleId, msg::Cluster &data, std::string &reason )
{
	msg::GetCluster query;
	query.m_moudle = moudleId;
	query.Build();
	ResultCode::ResultCode ret = Send(query, query.Size());
	if( ResultCode::success != ret )
	{
		return ret;
	}
	return ReadMsg(&data, 1);
}
