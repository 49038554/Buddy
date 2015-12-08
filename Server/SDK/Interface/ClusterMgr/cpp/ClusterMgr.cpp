#include "ClusterMgr.h"
#include "Protocl/cpp/Object/Operation/GetCluster.h"

ClusterMgr::ClusterMgr()
{
}

ClusterMgr::~ClusterMgr()
{
}

//取集群信息
SyncClient::ErrorCode ClusterMgr::GetCluster( Moudle::Moudle moudleId, msg::Cluster &data, std::string &reason )
{
	if ( !Connect() )
	{
		reason = "连接失败";
		return CONNECT_FAILED;
	}

	msg::GetCluster query;
	query.m_moudle = moudleId;
	query.Build();
	if( Send(query, query.Size()) < 0 )
	{
		reason = "发送报文失败";
		Close(true);
		return NET_ERROR;
	}
	if ( !RecvData(data) )
	{
		reason = "接收数据失败";
		return NET_ERROR;
	}
	if ( MsgId::cluster != data.Id() )
	{
		reason = "协议号错误";
		Close(true);
		return SVR_ERROR;
	}
	if ( !data.Parse() )
	{
		reason = "报文格式错误";
		Close(true);
		return SVR_ERROR;
	}
	Close();
	return SUCESS;
}
