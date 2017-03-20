#include "Worker.h"
#include "mdk/mapi.h"
#include "common/MysqlCluster.h"
#include "Protocl/cpp/Object/Operation/GetCluster.h"
#include "Protocl/cpp/Object/Operation/Cluster.h"
#include <cstdlib>
#include <cstring>


#ifdef WIN32
#ifdef _DEBUG
#pragma comment ( lib, "mdk_d.lib" )
#else
#pragma comment ( lib, "mdk.lib" )
#endif
#endif


Worker::Worker(void)
{
	m_log.SetLogName("ClusterMgr");
	m_log.SetPrintLog(true);

	char exeDir[256];
	mdk::GetExeDir( exeDir, 256 );//取得可执行程序位置
	char cfgFile[256];
	sprintf( cfgFile, "%s/ClusterMgr.cfg", exeDir );
	if ( !m_cfg.ReadConfig( cfgFile ) )
	{
		m_log.Info( "Error", "配置错误" );
		mdk::mdk_assert(false);
		exit(0);
	}
	Listen(m_cfg["opt"]["listen"]);
	unsigned int CPU = mdk::GetCUPNumber( 32, 32 );
	SetWorkThreadCount(CPU);
	std::vector<void*> objList;
	MysqlCluster *pCluster = NULL;
	int i = 0;
	for ( i = 0; i < CPU; i++ )
	{
		pCluster = new MysqlCluster();
		pCluster->Init(m_cfg);
		objList.push_back(pCluster);
	}
	SetThreadsObjects( objList );
}

Worker::~Worker(void)
{
}

void Worker::OnConnect(mdk::NetHost &host)
{
}

void Worker::OnCloseConnect(mdk::NetHost &host)
{

}

void Worker::OnMsg(mdk::NetHost &host)
{
	msg::Buffer buffer; 
	if ( !host.Recv(buffer, buffer.HeaderSize(), false) ) return;
	if ( !buffer.ReadHeader() )
	{
		if ( !host.IsServer() ) host.Close();
		m_log.Info("Error","报文头错误");
		return;
	}
	if ( !host.Recv(buffer, buffer.Size()) ) return;
	if ( Moudle::Operation != buffer.MoudleId() )
	{
		m_log.Info("Error", "不是运维模块请求");
		host.Close();
		return;
	}

	switch ( buffer.Id() )
	{
	case MsgId::getCluster :
		OnGetCluster(host, buffer);
		break;
	default:
		break;
	}
}

bool Worker::OnGetCluster(mdk::NetHost &host, msg::Buffer &buffer)
{
	if ( buffer.IsResult() ) 
	{
		m_log.Info("Error", "不应该收到GetCluster回应报文");
		host.Close();
		return false;
	}
	msg::GetCluster msg;
	memcpy(msg, buffer, buffer.Size());
	if ( !msg.Parse() ) 
	{
		m_log.Info("Error", "非法格式报文");
		host.Close();
		return false;
	}
	msg::Cluster nodes;
	nodes.m_cluster.clear();

	MysqlCluster *pCluster = (MysqlCluster*)SafeObject();
	MySqlClient *pMysql = pCluster->Node("Cluster");
	char sql[256];
	if ( Moudle::all == msg.m_moudle ) sprintf( sql, 
		"select node_type.id as moudleId, net_line.id as lineId, node.nodeNo as nodeId, node.ip, node.`port` "
		"from node, node_type, net_line "
		"where node.`name` = node_type.`name` and node.netLine = net_line.`name` "
		"order by nodeNo asc", msg.m_moudle );
	else sprintf( sql, 
		"select node_type.id as moudleId, net_line.id as lineId, node.nodeNo as nodeId, node.ip, node.`port` "
		"from node, node_type, net_line "
		"where node.`name` = node_type.`name` and node.netLine = net_line.`name` and node_type.id = %d "
		"order by nodeNo asc", msg.m_moudle );
	if ( !pMysql->ExecuteSql(sql) )
	{
		m_log.Info("Error", "访问数据库失败:%s", pMysql->GetLastError());
		m_log.Info("Error", "sql:%s", sql);
		host.Close();
		return false;
	}
	pMysql->MoveFirst();
	char val;
	Moudle::Moudle moudleId;
	NetLine::NetLine lineId;
	msg::Cluster::NODE node;
	while ( !pMysql->IsEof() )
	{
		pMysql->GetValue("moudleId", val);
		moudleId = (Moudle::Moudle)val;
		pMysql->GetValue("nodeId", node.nodeId);
		pMysql->GetValue("lineId", val);
		lineId = (NetLine::NetLine)val;
		pMysql->GetValue("ip", node.ip);
		pMysql->GetValue("port", node.port);
		nodes.m_cluster[moudleId][lineId].push_back(node);
		pMysql->MoveNext();
	}
	if ( !nodes.Build() ) 
	{
		m_log.Info("Error", "构造报文失败");
		host.Close();
		return false;
	}
	host.Send(nodes, nodes.Size());

	return true;
}
