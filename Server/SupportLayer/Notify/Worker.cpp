#include <cstdlib>
#include <cstring>
#include "Worker.h"
#include "mdk/mapi.h"
#include "Protocl/cpp/Object/Notify/Event.h"
#include "Protocl/cpp/Object/ConnectAuth.h"

#ifdef WIN32
#ifdef _DEBUG
#pragma comment ( lib, "mdk_d.lib" )
#pragma comment ( lib, "hiredis_d.lib" )
#else
#pragma comment ( lib, "mdk.lib" )
#pragma comment ( lib, "hiredis.lib" )
#endif
#endif


Worker::Worker(void)
{
	m_log.SetLogName("Notify");
	m_log.SetPrintLog(true);

	char exeDir[256];
	mdk::GetExeDir( exeDir, 256 );//ȡ�ÿ�ִ�г���λ��
	char cfgFile[256];
	sprintf( cfgFile, "%s/Notify.cfg", exeDir );
	if ( !m_cfg.ReadConfig( cfgFile ) )
	{
		m_log.Info( "Error", "���ô���" );
		mdk::mdk_assert(false);
		exit(0);
	}
	std::string ip = m_cfg["opt"]["ip"];
	int port = m_cfg["opt"]["listen"];
	m_cluster.SetSvr(m_cfg["ClusterMgr"]["ip"], m_cfg["ClusterMgr"]["port"]);
	msg::Cluster clusterInfo;
	std::string reason;
	m_log.Info( "Run", "��Ⱥ���÷���(%s %d)", ((std::string)m_cfg["ClusterMgr"]["ip"]).c_str(), 
		(int)m_cfg["ClusterMgr"]["port"] );
	if ( SyncClient::SUCESS != m_cluster.GetCluster(Moudle::all, clusterInfo, reason) )
	{
		m_log.Info( "Error", "��ȡ��Ⱥ��Ϣʧ��:%s", reason.c_str() );
		mdk::mdk_assert(false);
		exit(0);
	}
	NODE_INFO *pNode;
	std::map<Moudle::Moudle, std::map<NetLine::NetLine, std::vector<msg::Cluster::NODE> > >::iterator itMoudle;
	std::map<NetLine::NetLine, std::vector<msg::Cluster::NODE> >::iterator itLine;
	std::vector<msg::Cluster::NODE>::iterator it;
	int tcpEntryCount = 0;
	for ( itMoudle = clusterInfo.m_cluster.begin(); itMoudle != clusterInfo.m_cluster.end(); itMoudle++ )
	{
		itLine = itMoudle->second.begin();
		for ( ; itLine != itMoudle->second.end(); itLine++ )
		{
			for ( it = itLine->second.begin(); it != itLine->second.end(); it++ )
			{
				if ( Moudle::Notify == itMoudle->first )
				{
					if ( port == it->port )
					{
						if ( 0 == strcmp(ip.c_str(), it->ip.c_str()) )
						{
							m_nodeId = it->nodeId;
							m_cfg["opt"]["nodeId"] = m_nodeId;
							m_cfg.Save();
						}
					}
					continue;
				}
				else if ( Moudle::TcpEntry == itMoudle->first ) tcpEntryCount++;
				else continue;

				pNode = new NODE_INFO;
				pNode->nodeId = it->nodeId;
				Connect(it->ip.c_str(), it->port, pNode, 5);
			}
		}
	}
	if ( 0 >= tcpEntryCount )
	{
		m_log.Info( "Error", "��Ⱥȱ��ģ�飬����Cluster��" );
		mdk::mdk_assert(false);
		exit(0);
	}
	m_log.Info("Run", "Tcp��ڽ��%d��", tcpEntryCount);
	int CPU = mdk::GetCUPNumber(32, 32);
	SetWorkThreadCount(CPU);
	m_cache.InitCluster(m_cfg, CPU);

	m_log.Info( "Run", "�����˿�:%d", port );
	Listen(port);
}

Worker::~Worker(void)
{
}

void Worker::OnConnect(mdk::NetHost &host)
{
	if ( !host.IsServer() ) return;

	NODE_INFO *pSvr = (NODE_INFO*)host.GetSvrInfo();
	if ( NULL == pSvr ) return;

	mdk::AutoLock lock(&m_lockTcpEntryMap);
	m_tcpEntryMap[pSvr->nodeId] = host;
	msg::ConnectAuth auth;
	auth.m_nodeType = Moudle::Notify;
	auth.m_nodeId = m_nodeId;
	auth.Build();
	host.Send(auth, auth.Size());
}

void Worker::OnCloseConnect(mdk::NetHost &host)
{
	if ( !host.IsServer() ) return;

	NODE_INFO *pSvr = (NODE_INFO*)host.GetSvrInfo();
	if ( NULL == pSvr ) return;
	mdk::AutoLock lock(&m_lockTcpEntryMap);
	m_tcpEntryMap.erase(pSvr->nodeId);
}

void Worker::OnMsg(mdk::NetHost &host)
{
	msg::Buffer buffer;

	if ( !host.Recv(buffer, buffer.HeaderSize(), false) ) return;
	if ( 0 > buffer.Size() ) 
	{
		m_log.Info("Waring", "�Ƿ����ĳ���");
		host.Close();
		return;
	}

	if ( !host.Recv(buffer, buffer.Size()) ) return;
	if ( Moudle::Notify != buffer.MoudleId() 
		|| buffer.IsResult() 
		|| MsgId::event != buffer.Id() ) 
	{
		m_log.Info("Waring", "�Ƿ�����");
		return;
	}

	// �����û�ע�ᱨ��
	if (! buffer.Parse())
	{
		m_log.Info("Error", "�����û�����ʧ��!");
		return;
	}

	int tcpNodeId;
	m_cache.GetUserNode(buffer.m_objectId, tcpNodeId);
	mdk::NetHost tcpHost;
	mdk::AutoLock lock(&m_lockTcpEntryMap);
	if ( m_tcpEntryMap.end() == m_tcpEntryMap.find(tcpNodeId) ) 
	{
		m_log.Info("Waring", "TcpEntryδ����");
		return;
	}
	tcpHost = m_tcpEntryMap[tcpNodeId];
	tcpHost.Send(buffer, buffer.Size());
}
