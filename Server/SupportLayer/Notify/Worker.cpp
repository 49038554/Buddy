#include <cstdlib>
#include <cstring>
#include "Worker.h"
#include "mdk/mapi.h"
#include "Protocl/cpp/Object/Notify/Event.h"
#include "Protocl/cpp/Object/Notify/GetEvent.h"
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
	mdk::GetExeDir( exeDir, 256 );//取得可执行程序位置
	char cfgFile[256];
	sprintf( cfgFile, "%s/Notify.cfg", exeDir );
	if ( !m_cfg.ReadConfig( cfgFile ) )
	{
		m_log.Info( "Error", "配置错误" );
		mdk::mdk_assert(false);
		exit(0);
	}
	std::string ip = m_cfg["opt"]["ip"];
	int port = m_cfg["opt"]["listen"];
	m_cluster.SetSvr(m_cfg["ClusterMgr"]["ip"], m_cfg["ClusterMgr"]["port"]);
	msg::Cluster clusterInfo;
	std::string reason;
	m_log.Info( "Run", "集群配置服务(%s %d)", ((std::string)m_cfg["ClusterMgr"]["ip"]).c_str(), 
		(int)m_cfg["ClusterMgr"]["port"] );
	if ( SyncClient::SUCESS != m_cluster.GetCluster(Moudle::all, clusterInfo, reason) )
	{
		m_log.Info( "Error", "获取集群信息失败:%s", reason.c_str() );
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
							m_cfg["Only Id"]["nodeId"] = m_nodeId;
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
		m_log.Info( "Error", "集群缺少模块，请检查Cluster库" );
		mdk::mdk_assert(false);
		exit(0);
	}
	m_log.Info("Run", "Tcp入口结点%d个", tcpEntryCount);
	int CPU = mdk::GetCUPNumber(32, 32);
	SetWorkThreadCount(CPU);
	m_cache.InitCluster(m_cfg, CPU);

	InitId("Notify.cfg");
	m_log.Info( "Run", "监听端口:%d", port );
	Listen(port);
}

Worker::~Worker(void)
{
}

bool Worker::InitId( const std::string &fileName )
{
	char exePath[1024];
	mdk::GetExeDir( exePath, 1024 );//取得可执行程序位置
	char cfgFile[1024+256];
	sprintf( cfgFile, "%s/%s", exePath, fileName.c_str() );
	m_pCfg = new mdk::ConfigFile();

	mdk::ConfigFile &cfg = *m_pCfg;
	if ( !cfg.ReadConfig( cfgFile ) ) return false;
	m_nodeId = cfg["Only Id"]["nodeId"];
	std::string searialNo = cfg["Only Id"]["searialNo"];
	m_searialNo = 0;
	sscanf( searialNo.c_str(), "%u", &m_searialNo );

	return true;
}

bool Worker::CreateId( mdk::uint32 &id, bool now )
{
	//id已用完
	if ( 0x01000000 == m_searialNo ) return false;

	id = m_nodeId;
	id <<= 24;
	id += m_searialNo;
	m_searialNo++;
	SaveId(false);

	return true;
}

void Worker::SaveId( bool now )
{
	static uint64 count = 0;
	static uint64 lastSave = 0;
	mdk::ConfigFile &cfg = *m_pCfg;

	count++;
	if ( now || 0 == count % 10000
		|| mdk::MillTime() - lastSave > 5000 )
	{
		lastSave = mdk::MillTime();
		char temp[128];
		sprintf( temp, "%u", m_searialNo );
		cfg["Only Id"]["searialNo"] = (std::string)temp;
		cfg.Save();
	}
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
		m_log.Info("Waring", "非法报文长度");
		host.Close();
		return;
	}

	if ( !host.Recv(buffer, buffer.Size()) ) return;
	if ( Moudle::Notify != buffer.MoudleId() 
		|| buffer.IsResult() ) 
	{
		m_log.Info("Waring", "非法报文");
		return;
	}

	// 解析用户注册报文
	if (! buffer.Parse())
	{
		m_log.Info("Error", "解析用户报文失败!");
		return;
	}

	if ( MsgId::event == buffer.Id() ) OnEvent(host, buffer);
	if ( MsgId::getEvent == buffer.Id() ) OnGetEvent(host, buffer);
}

void Worker::OnEvent(mdk::NetHost &host, msg::Buffer &buffer)
{
	msg::Event msg;
	memcpy(msg, buffer, buffer.Size());
	if ( !msg.Parse() )
	{
		m_log.Info("Error", "非法报文格式");
		return;
	}
	if ( !CreateId( msg.m_eventId ) )
	{
		m_log.Info("Error", "无可用Id");
		return;
	}
	msg.Build();
	int tcpNodeId;
	if ( Redis::success != m_cache.GetUserNode(buffer.m_objectId, tcpNodeId) )//取不到用户连接信息，则保存下来以后发送
	{
		if ( !m_cache.AddEvent(msg.m_objectId, msg.m_eventId, msg, msg.Size()) )
		{
			m_log.Info("Error", "保存通知失败");
		}
		return;
	}

	//转发出去
	mdk::NetHost tcpHost;
	mdk::AutoLock lock(&m_lockTcpEntryMap);
	if ( m_tcpEntryMap.end() == m_tcpEntryMap.find(tcpNodeId) ) 
	{
		m_log.Info("Waring", "TcpEntry未连接");
		return;
	}
	tcpHost = m_tcpEntryMap[tcpNodeId];
	tcpHost.Send(buffer, buffer.Size());
}

void Worker::OnGetEvent(mdk::NetHost &host, msg::Buffer &buffer)
{
	std::vector<std::string> events;
	Redis::Result ret = m_cache.GetEvents(buffer.m_objectId, events);

	if ( Redis::unsvr == ret )
	{
		m_log.Info("Error", "读取通知失败:Redis服务异常");
		return;
	}
	if ( 0 >= events.size() ) return;

	//转发出去
	int i = 0;
	for ( i = 0; i < events.size(); i++ )
	{
		buffer.ReInit();
		memcpy(buffer, events[i].c_str(), events[i].size());
		if ( !buffer.Parse() ) 
		{
			m_log.Info("Error", "1条消息无法解析");
			continue;
		}
		host.Send(buffer, buffer.Size());
	}
}
