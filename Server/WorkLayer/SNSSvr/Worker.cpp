#include "Worker.h"
#include "mdk/mapi.h"
#include "Interface/DBCenter/cpp/DBCenterCluster.h"
#include "Protocl/cpp/Object/SNS/AddBuddy.h"
#include "Protocl/cpp/Object/Notify/Event.h"

#ifdef WIN32
#ifdef _DEBUG
#pragma comment(lib, "mdk_d.lib")
#pragma comment(lib, "hiredis_d.lib")
#else
#pragma comment(lib, "mdk.lib")
#pragma comment(lib, "hiredis.lib")
#endif // _DEBUG
#endif // WIN32

Worker::Worker(void)
{
	// 设置Log相关
	m_log.SetLogName("SNSSvr");
	m_log.SetPrintLog(true);

	// 读取配置文件
	const int nBufferSize      = 256;
	char pExeDir[nBufferSize]  = {0};
	char pCfgFile[nBufferSize] = {0};

	// 取得可执行程序的位置
	mdk::GetExeDir(pExeDir, nBufferSize);
	// 取得配置文件的位置
	sprintf(pCfgFile, "%s/SNSSvr.cfg", pExeDir);

	if (! m_cfg.ReadConfig(pCfgFile))
	{
		m_log.Info("Error", "SNSSvr.cfg 配置错误!");
		mdk::mdk_assert(false);
		exit(0);
	}

	std::string curSvrIP   = m_cfg["opt"]["ip"];
	int         curSvrPORT = m_cfg["opt"]["listen"];

	// 从集群配置服获取集群配置信息
	m_cluster.SetSvr(m_cfg["ClusterMgr"]["ip"], m_cfg["ClusterMgr"]["port"]);
	m_log.Info("Run", "集群配置服务(%s %d)", std::string(m_cfg["ClusterMgr"]["ip"]).c_str(), int(m_cfg["ClusterMgr"]["port"]));

	msg::Cluster clusterInfo;
	std::string  reason;
	if (SyncClient::SUCESS != m_cluster.GetCluster(Moudle::all, clusterInfo, reason))
	{
		m_log.Info( "Error", "获取集群信息失败:%s", reason.c_str() );
		mdk::mdk_assert(false);
		exit(0);
	}

	ConnectInfo *pNode = NULL;
	std::vector<msg::Cluster::NODE> dbCenters;
	std::map< Moudle::Moudle, std::map< NetLine::NetLine, std::vector<msg::Cluster::NODE> > >::iterator itMoudle;
	std::map< NetLine::NetLine, std::vector<msg::Cluster::NODE> >::iterator itLine;
	std::vector<msg::Cluster::NODE>::iterator itNode;
	for (itMoudle = clusterInfo.m_cluster.begin(); itMoudle != clusterInfo.m_cluster.end(); ++itMoudle)
	{
		for (itLine = itMoudle->second.begin(); itLine != itMoudle->second.end(); ++itLine)
		{
			for (itNode = itLine->second.begin(); itNode != itLine->second.end(); ++itNode)
			{
				// 依据集群信息修正本服务器的节点ID
				if (Moudle::SNS == itMoudle->first && curSvrPORT == itNode->port && curSvrIP == itNode->ip)
				{
					m_cfg["opt"]["nodeId"] = itNode->nodeId;
					m_cfg.Save();
				}
				if (Moudle::DBEntry == itMoudle->first) 
				{
					dbCenters.push_back(*itNode);
					continue;
				}
				if (Moudle::Notify == itMoudle->first) ;
				else continue;

				ConnectInfo * pNode = new ConnectInfo;
				pNode->nodeId = itNode->nodeId;
				pNode->type = itMoudle->first;
				pNode->lineType = itLine->first;
				pNode->ip = itNode->ip;
				pNode->port = itNode->port;
				Connect(pNode->ip.c_str(), pNode->port, pNode, 5);
			}
		}
	}
	m_log.Info("Run", "DB服务器节点%d个", dbCenters.size());

	// 设置工作线程数目
	unsigned uNumbers = mdk::GetCUPNumber( 32, 4 );
	SetWorkThreadCount(uNumbers);
	std::vector<void*> objList;
	int i = 0;
	for ( i = 0; i < uNumbers; i++ )
	{
		DBCenterCluster *pCluster = new DBCenterCluster;
		int j = 0;
		for ( j = 0; j < dbCenters.size(); j++ )
		{
			pCluster->AddNode(dbCenters[j].nodeId, dbCenters[j].ip, dbCenters[j].port);
		}
		objList.push_back(pCluster);
		if ( 0 < i ) continue;
		if ( !pCluster->CheckCluster() )
		{
			m_log.Info("run", "DBCenter集群配置异常：结点id必须从1开始且连续");
			mdk::mdk_assert(false);
			exit(0);
			return;
		}
	}
	SetThreadsObjects(objList);
	// 初始化Redis集群
	if (! m_cache.InitCluster(m_cfg, uNumbers))
	{
		m_log.Info("Error", "Redis集群初始化失败！");
		mdk::mdk_assert(false);
		exit(EXIT_FAILURE);
	}
	
	// 播随机种子
	srand(unsigned(time(NULL)));

	// 监听端口，等待连接
	m_log.Info("Run", "监听端口:%d", curSvrPORT);
	Listen(curSvrPORT);
}

Worker::~Worker(void)
{
}

void Worker::OnConnect(mdk::NetHost &host)
{
	if ( !host.IsServer() )
	{
		std::string ip;
		int port;
		host.GetAddress(ip, port);
		m_log.Info("Run", "client(%s:%d):连接", ip.c_str(), port);
		return;
	}

	ConnectInfo *pInfo = (ConnectInfo*)host.GetSvrInfo();
	if ( Moudle::Notify == pInfo->type )
	{
		m_notifyCluster.AddNode(pInfo->nodeId, host);
		m_log.Info("run", "Notify(%d):%s:%d连接完成", pInfo->nodeId, pInfo->ip.c_str(), pInfo->port);
	}
}

void Worker::OnCloseConnect(mdk::NetHost &host)
{
	if ( !host.IsServer() )
	{
		std::string ip;
		int port;
		host.GetAddress(ip, port);
		m_log.Info("Run", "client(%s:%d):断开", ip.c_str(), port);
		return;
	}

	ConnectInfo *pInfo = (ConnectInfo*)host.GetSvrInfo();
	if ( Moudle::Notify == pInfo->type )
	{
		m_notifyCluster.DelNode(pInfo->nodeId);
		m_log.Info("run", "Notify(%d):%s:%d断开", pInfo->nodeId, pInfo->ip.c_str(), pInfo->port);
	}
}

void Worker::OnMsg(mdk::NetHost& host)
{
	msg::Buffer buffer;
	if (! host.Recv(buffer, buffer.HeaderSize(), false)) return;
	if (-1 == buffer.Size())
	{
		m_log.Info("Error", "非法报文,断开连接!!!" );
		host.Close();
		return;
	}
	if (! host.Recv(buffer, buffer.Size())) return;
	if ( buffer.IsResult() )
	{
		m_log.Info("Error", "未预料的回应,断开连接!!!" );
		host.Close();
		return;
	}

	// 按照msgId进行消息处理，并过滤掉忽略的消息，记入日志
	int msgId = buffer.Id();
	if (MsgId::addBuddy == msgId) OnAddBuddy(host, buffer);
	else
	{
		std::string strIP;  // IP
		int         nPort;  // Port
		host.GetAddress(strIP, nPort);
		m_log.Info("Warning", "忽略报文!!! IP = %s, PORT = %d, msgId = %d", strIP.c_str(), nPort, msgId);
	}
}

bool Worker::OnAddBuddy(mdk::NetHost& host, msg::Buffer& buffer)
{
	msg::AddBuddy msg;
	memcpy(msg, buffer, buffer.Size());

	if ( !msg.Parse() )
	{
		msg.m_code   = ResultCode::FormatInvalid;
		msg.m_reason = "报文格式非法";
		msg.Build(true);
		host.Send(msg, msg.Size());
		return true;
	}
	mdk::NetHost notifyHost;
	int nodeId = m_notifyCluster.Node(notifyHost);
	if ( 0 == nodeId )
	{
		m_log.Info("Error", "添加小伙伴失败：消息中心无可用服务");
		return true;
	}
	if ( 0 == msg.m_step )
	{
		Cache::IdList buddyIds;
		if ( Redis::unsvr == m_cache.GetBuddys(msg.m_objectId, buddyIds) )
		{
			m_log.Info("Error", "添加小伙伴失败：Redis服务异常");
			return true;
		}
		int i = 0;
		for ( i = 0; i < buddyIds.m_ids.size() ; i++ )
		{
			if ( msg.m_buddyId == buddyIds.m_ids[i] )
			{
				msg.m_code   = ResultCode::Refuse;
				msg.m_reason = "已经是小伙伴";
				msg.Build(true);
				host.Send(msg, msg.Size());
				return true;
			}
		}
	}

	Cache::User ui;
	ui.id = msg.m_objectId;
	if ( Redis::success != m_cache.GetUserInfo(ui) )
	{
		m_log.Info("Error", "添加小伙伴失败：读用户信息失败");
		return true;
	}
	msg.m_userId = ui.id;
	msg.m_nickName = ui.nickName;
	msg.Build();
	msg::Event notify;
	notify.m_objectId = msg.m_buddyId;
	notify.m_senderId = msg.m_userId;
	notify.m_sender = msg.m_nickName;
	notify.m_holdTime = time(NULL) + 86400 * 31;
	if ( 1 == msg.m_step )//对方回应
	{
		if ( msg.m_accepted )//对方接受
		{
			DBCenter *pDBCenter = ((DBCenterCluster*)SafeObject())->Node(msg.m_objectId);
			pDBCenter->AddBuddy(msg);
		}
		else //对方拒绝
		{
			msg.m_code = ResultCode::Refuse;
			msg.m_reason = "对方拒绝";
			msg.Build(true);
		}
	}
	memcpy(notify.m_msg, msg, msg.Size());
	notify.Build();
	notifyHost.Send(notify, notify.Size());

	return true;
}
