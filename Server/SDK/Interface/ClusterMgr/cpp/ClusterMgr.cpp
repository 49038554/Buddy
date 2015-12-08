#include "ClusterMgr.h"
#include "Protocl/cpp/Object/Operation/GetCluster.h"

ClusterMgr::ClusterMgr()
{
}

ClusterMgr::~ClusterMgr()
{
}

//ȡ��Ⱥ��Ϣ
SyncClient::ErrorCode ClusterMgr::GetCluster( Moudle::Moudle moudleId, msg::Cluster &data, std::string &reason )
{
	if ( !Connect() )
	{
		reason = "����ʧ��";
		return CONNECT_FAILED;
	}

	msg::GetCluster query;
	query.m_moudle = moudleId;
	query.Build();
	if( Send(query, query.Size()) < 0 )
	{
		reason = "���ͱ���ʧ��";
		Close(true);
		return NET_ERROR;
	}
	if ( !RecvData(data) )
	{
		reason = "��������ʧ��";
		return NET_ERROR;
	}
	if ( MsgId::cluster != data.Id() )
	{
		reason = "Э��Ŵ���";
		Close(true);
		return SVR_ERROR;
	}
	if ( !data.Parse() )
	{
		reason = "���ĸ�ʽ����";
		Close(true);
		return SVR_ERROR;
	}
	Close();
	return SUCESS;
}
