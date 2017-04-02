#include "Worker.h"
#include "mdk/mapi.h"
#include "Interface/DBCenter/cpp/DBCenterCluster.h"
#include "Protocl/cpp/Object/Notify/Event.h"

#include "Protocl/cpp/Object/DBEntry/SetupVersion.h"
#include "Protocl/cpp/Object/DBEntry/SkillBook.h"
#include "Protocl/cpp/Object/DBEntry/ItemBook.h"
#include "Protocl/cpp/Object/DBEntry/BuddyBook.h"
#include "Protocl/cpp/Object/DBEntry/BuddyMap.h"
#include "Protocl/cpp/Object/DBEntry/RaceMap.h"

//#include "Protocl/cpp/Object/Game/CreatePlayer.h"
//#include "Protocl/cpp/Object/Game/KillBeast.h"
//#include "Protocl/cpp/Object/Game/UseItem.h"
// #include "Protocl/cpp/Object/Game/GrowUp.h"
// #include "Protocl/cpp/Object/Game/TameBeast.h"
// #include "Protocl/cpp/Object/Game/BuildHouse.h"
// #include "Protocl/cpp/Object/Game/PutPet.h"
// #include "Protocl/cpp/Object/Game/TreePlant.h"
// #include "Protocl/cpp/Object/Game/Pick.h"
// #include "Protocl/cpp/Object/Game/NearInfo.h"
// #include "Protocl/cpp/Object/Game/CreateShop.h"
// #include "Protocl/cpp/Object/Game/Buy.h"
// #include "Protocl/cpp/Object/Game/Devour.h"
// #include "Protocl/cpp/Object/Game/CatchStar.h"
// #include "Protocl/cpp/Object/Game/CityBuildInit.h"
// #include "Protocl/cpp/Object/Game/CityBuilding.h"
// #include "Protocl/cpp/Object/Game/DelHouse.h"

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
	m_log.SetLogName("GameSvr");
	m_log.SetPrintLog(true);

	// 读取配置文件
	const int nBufferSize      = 256;
	char pExeDir[nBufferSize]  = {0};
	char pCfgFile[nBufferSize] = {0};

	// 取得可执行程序的位置
	mdk::GetExeDir(pExeDir, nBufferSize);
	// 取得配置文件的位置
	sprintf(pCfgFile, "%s/GameSvr.cfg", pExeDir);

	if (! m_cfg.ReadConfig(pCfgFile))
	{
		m_log.Info("Error", "GameSvr.cfg 配置错误!");
		mdk::mdk_assert(false);
		exit(0);
	}

	std::string curSvrIP   = m_cfg["opt"]["ip"];
	int         curSvrPORT = m_cfg["opt"]["listen"];

	// 从集群配置服获取集群配置信息
	m_cluster.SetService(m_cfg["ClusterMgr"]["ip"], m_cfg["ClusterMgr"]["port"]);
	m_log.Info("Run", "集群配置服务(%s %d)", std::string(m_cfg["ClusterMgr"]["ip"]).c_str(), int(m_cfg["ClusterMgr"]["port"]));

	msg::Cluster clusterInfo;
	std::string  reason;
	if (ResultCode::success != m_cluster.GetCluster(Moudle::all, clusterInfo, reason))
	{
		m_log.Info( "Error", "获取集群信息失败:%s", reason.c_str() );
		mdk::mdk_assert(false);
		exit(0);
	}

	int notifyCount = 0;
	ConnectInfo *pNode = NULL;
	std::vector<msg::Cluster::NODE> dbCenters;
	std::map< Moudle::Moudle, std::map< NetLine::NetLine, std::vector<msg::Cluster::NODE> > >::iterator itMoudle;
	std::map< NetLine::NetLine, std::vector<msg::Cluster::NODE> >::iterator itLine;
	std::vector<msg::Cluster::NODE>::iterator itNode;
	bool initNodeId = false;
	for (itMoudle = clusterInfo.m_cluster.begin(); itMoudle != clusterInfo.m_cluster.end(); ++itMoudle)
	{
		for (itLine = itMoudle->second.begin(); itLine != itMoudle->second.end(); ++itLine)
		{
			for (itNode = itLine->second.begin(); itNode != itLine->second.end(); ++itNode)
			{
				// 依据集群信息修正本服务器的节点ID
				if (Moudle::Game == itMoudle->first && curSvrPORT == itNode->port && curSvrIP == itNode->ip)
				{
					m_cfg["opt"]["nodeId"] = m_nodeId = itNode->nodeId;
					m_cfg.Save();
					initNodeId = true;
				}
				if (Moudle::DBEntry == itMoudle->first) 
				{
					dbCenters.push_back(*itNode);
					continue;
				}
				if (Moudle::Notify == itMoudle->first) notifyCount++;
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
	if ( !initNodeId )
	{
		m_log.Info("Error", "结点Id配置错误！");
		mdk::mdk_assert(false);
	}
	m_log.Info("Run", "DB服务器节点%d个，消息中心节点%d个", dbCenters.size(), notifyCount );
	m_notifyCluster.SetNodeCount(notifyCount);
	// 设置工作线程数目
	DBCenterCluster *pCluster = NULL;
	unsigned uNumbers = mdk::GetCUPNumber( 32, 4 );
	SetWorkThreadCount(uNumbers);
	std::vector<void*> objList;
	int i = 0;
	for ( i = 0; i < uNumbers; i++ )
	{
		pCluster = new DBCenterCluster;
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
	//检查本地是否有数据，没有则从数据层读取
	DBCenter *pDBCenter = pCluster->Node(m_nodeId);
	if ( !pDBCenter->GetGameSetupData( m_dataVersion, m_races, m_skills, m_items, m_buddys, m_buddyMaps) )
	{
		m_log.Info("run", "获取安装数据失败");
		mdk::mdk_assert(false);
		exit(0);
		return;
	}

	// 初始化Redis集群
	if (! m_cache.InitCluster(m_cfg, uNumbers))
	{
		m_log.Info("Error", "Redis集群初始化失败！");
		mdk::mdk_assert(false);
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
	if ( !host.Recv(buffer, buffer.HeaderSize(), false) ) return;
	if ( !buffer.ReadHeader() )
	{
		if ( !host.IsServer() ) host.Close();
		m_log.Info("Error","报文头错误");
		return;
	}
	if ( !host.Recv(buffer, buffer.Size()) ) return;
	if ( buffer.IsResult() )
	{
		m_log.Info("Error", "未预料的回应,断开连接!!!" );
		host.Close();
		return;
	}

	// 按照msgId进行消息处理，并过滤掉忽略的消息，记入日志
	int msgId = buffer.Id();
	switch ( msgId )
	{
		case MsgId::setupVersion://安装版本,地图数据，巴迪图鉴，物品图鉴
			OnSetupVersion(host, buffer);
			break;
// 		case MsgId::createPlayer://创建玩家
// 			OnCreatePlayer(host, buffer);
// 			break;
// 		case MsgId::killBeast://杀死野怪
// 			OnKillBeast(host, buffer);
// 			break;
// 		case MsgId::useItem://使用物品
// 			OnUseItem(host, buffer);
// 			break;
// 		case MsgId::growUp://进化
// 			OnGrowUp(host, buffer);
// 			break;
// 		case MsgId::tameBeast://驯服野怪
// 			OnTameBeast(host, buffer);
// 			break;
// 		case MsgId::buildHouse://地图技能：光墙，造房子
// 			OnBuildHouse(host, buffer);
// 			break;
// 		case MsgId::putPet://放置宠物
// 			OnPutPet(host, buffer);
// 			break;
// 		case MsgId::treePlant://种树
// 			OnTreePlant(host, buffer);
// 			break;
// 		case MsgId::pick://采摘
// 			OnPick(host, buffer);
// 			break;
// 		case MsgId::nearInfo://周边信息
// 			OnNearInfo(host, buffer);
// 			break;
// 		case MsgId::createShop://开店
// 			OnCreateShop(host, buffer);
// 			break;
// 		case MsgId::buy://购买
// 			OnBuy(host, buffer);
// 			break;
// 		case MsgId::devour://地图技能：吞噬，物品转换正能量
// 			OnDevour(host, buffer);
// 			break;
// 		case MsgId::catchStar://地图技能：摘星术，将天上的法宝打下来
// 			OnCatchStar(host, buffer);
// 			break;
// 		case MsgId::cityBuildInit://开始城市建设
// 			OnCityBuildInit(host, buffer);
// 			break;
// 		case MsgId::cityBuilding://为城市建设添砖加瓦
// 			OnCityBuilding(host, buffer);
// 			break;
// 		case MsgId::delHouse://地图技能：瓦割，拆房子
// 			OnDelHouse(host, buffer);
// 			break;
		default:
			break;
	}
}

bool Worker::SendNotify(mdk::uint32 recverId, unsigned char recvType, msg::Buffer *pMsg, time_t holdTime, mdk::uint32 sender, const std::string &senderName)
{
	mdk::NetHost notifyHost;
	int nodeId = m_notifyCluster.Node(recverId, notifyHost);
	if ( 0 == nodeId ) return false;

	msg::Event notify;
	notify.m_senderId = sender;
	notify.m_sender = senderName;
	notify.m_recvType = (msg::Event::RecvType)recvType;
	notify.m_recverId = recverId;
	notify.m_holdTime = holdTime;
	memcpy(notify.m_msg, *pMsg, pMsg->Size());
	notify.Build();
	notifyHost.Send(notify, notify.Size());

	return true;
}

void Worker::OnSetupVersion(mdk::NetHost &host, msg::Buffer &buf)
{
	msg::SetupVersion msg;
	memcpy(msg, buf, buf.Size());
	if ( !msg.Parse() )
	{
		msg.m_code = ResultCode::msgError;
		msg.m_reason = "非法报文格式";
		msg.Build(true);
		host.Send(msg, msg.Size());
		return;
	}

	if ( msg.m_dataVersion == m_dataVersion )
	{
		msg.m_code = ResultCode::success;
		msg.Build(true);
		host.Send(msg, msg.Size());
		return;
	}

	//如果client版本与服务不匹配，则发送新版数据
	msg.m_dataVersion = m_dataVersion;
	{
		msg::RaceMap msg;
		msg.m_races = m_races;
		msg.Build();
		host.Send(msg, msg.Size());
	}
	{
		msg::SkillBook msg;
		int i = 0;
		int j = 0;
		while ( i < m_skills.size() )
		{
			msg.m_skills.clear();
			for ( j = 0; j < 50 && i < m_skills.size(); j++ )
			{
				msg.m_skills.push_back(m_skills[i]);
				i++;
			}
			msg.Build();
			host.Send(msg, msg.Size());
		}
	}
	{
		msg::ItemBook msg;
		int i = 0;
		int j = 0;
		while ( i < m_items.size() )
		{
			msg.m_items.clear();
			for ( j = 0; j < 50 && i < m_items.size(); j++ )
			{
				msg.m_items.push_back(m_items[i]);
				i++;
			}
			msg.Build();
			host.Send(msg, msg.Size());
		}
	}
	{
		msg::BuddyBook msg;
		int i = 0;
		int j = 0;
		while ( i < m_buddyMaps.size() )
		{
			msg.m_buddys.clear();
			for ( j = 0; j < 50 && i < m_buddys.size(); j++ )
			{
				msg.m_buddys.push_back(m_buddys[i]);
				i++;
			}
			msg.Build();
			host.Send(msg, msg.Size());
		}
	}
	{
		msg::BuddyMap msg;
		int i = 0;
		int j = 0;
		while ( i < m_buddyMaps.size() )
		{
			msg.m_buddyMaps.clear();
			for ( j = 0; j < 10 && i < m_buddyMaps.size(); j++ )
			{
				msg.m_buddyMaps.push_back(m_buddyMaps[i]);
				i++;
			}
			msg.Build();
			host.Send(msg, msg.Size());
		}
	}

	msg.m_code = ResultCode::success;
	msg.Build(true);
	host.Send(msg, msg.Size());

	return;
}

// void Worker::OnCreatePlayer(mdk::NetHost &host, msg::Buffer &buf)
// {
// 	msg::SetupVersion msg;
// 	memcpy(msg, buf, buf.Size());
// 	if ( !msg.Parse() )
// 	{
// 		msg.m_code = ResultCode::FormatInvalid;
// 		msg.m_reason = "非法报文格式";
// 		msg.Build(true);
// 		host.Send(msg, msg.Size());
// 		return;
// 	}
// }
// 
// void Worker::OnKillBeast(mdk::NetHost &host, msg::Buffer &buf)
// {
// 	msg::SetupVersion msg;
// 	memcpy(msg, buf, buf.Size());
// 	if ( !msg.Parse() )
// 	{
// 		msg.m_code = ResultCode::FormatInvalid;
// 		msg.m_reason = "非法报文格式";
// 		msg.Build(true);
// 		host.Send(msg, msg.Size());
// 		return;
// 	}
// }
// 
// void Worker::OnUseItem(mdk::NetHost &host, msg::Buffer &buf)
// {
// 	msg::SetupVersion msg;
// 	memcpy(msg, buf, buf.Size());
// 	if ( !msg.Parse() )
// 	{
// 		msg.m_code = ResultCode::FormatInvalid;
// 		msg.m_reason = "非法报文格式";
// 		msg.Build(true);
// 		host.Send(msg, msg.Size());
// 		return;
// 	}
// }
// 
// void Worker::OnGrowUp(mdk::NetHost &host, msg::Buffer &buf)
// {
// 	msg::SetupVersion msg;
// 	memcpy(msg, buf, buf.Size());
// 	if ( !msg.Parse() )
// 	{
// 		msg.m_code = ResultCode::FormatInvalid;
// 		msg.m_reason = "非法报文格式";
// 		msg.Build(true);
// 		host.Send(msg, msg.Size());
// 		return;
// 	}
// }
// 
// void Worker::OnTameBeast(mdk::NetHost &host, msg::Buffer &buf)
// {
// 	msg::SetupVersion msg;
// 	memcpy(msg, buf, buf.Size());
// 	if ( !msg.Parse() )
// 	{
// 		msg.m_code = ResultCode::FormatInvalid;
// 		msg.m_reason = "非法报文格式";
// 		msg.Build(true);
// 		host.Send(msg, msg.Size());
// 		return;
// 	}
// }
// 
// void Worker::OnBuildHouse(mdk::NetHost &host, msg::Buffer &buf)
// {
// 	msg::SetupVersion msg;
// 	memcpy(msg, buf, buf.Size());
// 	if ( !msg.Parse() )
// 	{
// 		msg.m_code = ResultCode::FormatInvalid;
// 		msg.m_reason = "非法报文格式";
// 		msg.Build(true);
// 		host.Send(msg, msg.Size());
// 		return;
// 	}
// }
// 
// void Worker::OnPutPet(mdk::NetHost &host, msg::Buffer &buf)
// {
// 	msg::SetupVersion msg;
// 	memcpy(msg, buf, buf.Size());
// 	if ( !msg.Parse() )
// 	{
// 		msg.m_code = ResultCode::FormatInvalid;
// 		msg.m_reason = "非法报文格式";
// 		msg.Build(true);
// 		host.Send(msg, msg.Size());
// 		return;
// 	}
// }
// 
// void Worker::OnTreePlant(mdk::NetHost &host, msg::Buffer &buf)
// {
// 	msg::SetupVersion msg;
// 	memcpy(msg, buf, buf.Size());
// 	if ( !msg.Parse() )
// 	{
// 		msg.m_code = ResultCode::FormatInvalid;
// 		msg.m_reason = "非法报文格式";
// 		msg.Build(true);
// 		host.Send(msg, msg.Size());
// 		return;
// 	}
// }
// 
// void Worker::OnPick(mdk::NetHost &host, msg::Buffer &buf)
// {
// 	msg::SetupVersion msg;
// 	memcpy(msg, buf, buf.Size());
// 	if ( !msg.Parse() )
// 	{
// 		msg.m_code = ResultCode::FormatInvalid;
// 		msg.m_reason = "非法报文格式";
// 		msg.Build(true);
// 		host.Send(msg, msg.Size());
// 		return;
// 	}
// }
// 
// void Worker::OnNearInfo(mdk::NetHost &host, msg::Buffer &buf)
// {
// 	msg::SetupVersion msg;
// 	memcpy(msg, buf, buf.Size());
// 	if ( !msg.Parse() )
// 	{
// 		msg.m_code = ResultCode::FormatInvalid;
// 		msg.m_reason = "非法报文格式";
// 		msg.Build(true);
// 		host.Send(msg, msg.Size());
// 		return;
// 	}
// }
// 
// void Worker::OnCreateShop(mdk::NetHost &host, msg::Buffer &buf)
// {
// 	msg::SetupVersion msg;
// 	memcpy(msg, buf, buf.Size());
// 	if ( !msg.Parse() )
// 	{
// 		msg.m_code = ResultCode::FormatInvalid;
// 		msg.m_reason = "非法报文格式";
// 		msg.Build(true);
// 		host.Send(msg, msg.Size());
// 		return;
// 	}
// }
// 
// void Worker::OnBuy(mdk::NetHost &host, msg::Buffer &buf)
// {
// 	msg::SetupVersion msg;
// 	memcpy(msg, buf, buf.Size());
// 	if ( !msg.Parse() )
// 	{
// 		msg.m_code = ResultCode::FormatInvalid;
// 		msg.m_reason = "非法报文格式";
// 		msg.Build(true);
// 		host.Send(msg, msg.Size());
// 		return;
// 	}
// }
// 
// void Worker::OnDevour(mdk::NetHost &host, msg::Buffer &buf)
// {
// 	msg::SetupVersion msg;
// 	memcpy(msg, buf, buf.Size());
// 	if ( !msg.Parse() )
// 	{
// 		msg.m_code = ResultCode::FormatInvalid;
// 		msg.m_reason = "非法报文格式";
// 		msg.Build(true);
// 		host.Send(msg, msg.Size());
// 		return;
// 	}
// }
// 
// void Worker::OnCatchStar(mdk::NetHost &host, msg::Buffer &buf)
// {
// 	msg::SetupVersion msg;
// 	memcpy(msg, buf, buf.Size());
// 	if ( !msg.Parse() )
// 	{
// 		msg.m_code = ResultCode::FormatInvalid;
// 		msg.m_reason = "非法报文格式";
// 		msg.Build(true);
// 		host.Send(msg, msg.Size());
// 		return;
// 	}
// }
// 
// void Worker::OnCityBuildInit(mdk::NetHost &host, msg::Buffer &buf)
// {
// 	msg::SetupVersion msg;
// 	memcpy(msg, buf, buf.Size());
// 	if ( !msg.Parse() )
// 	{
// 		msg.m_code = ResultCode::FormatInvalid;
// 		msg.m_reason = "非法报文格式";
// 		msg.Build(true);
// 		host.Send(msg, msg.Size());
// 		return;
// 	}
// }
// 
// void Worker::OnCityBuilding(mdk::NetHost &host, msg::Buffer &buf)
// {
// 	msg::SetupVersion msg;
// 	memcpy(msg, buf, buf.Size());
// 	if ( !msg.Parse() )
// 	{
// 		msg.m_code = ResultCode::FormatInvalid;
// 		msg.m_reason = "非法报文格式";
// 		msg.Build(true);
// 		host.Send(msg, msg.Size());
// 		return;
// 	}
// }
// 
// void Worker::OnDelHouse(mdk::NetHost &host, msg::Buffer &buf)
// {
// 	msg::SetupVersion msg;
// 	memcpy(msg, buf, buf.Size());
// 	if ( !msg.Parse() )
// 	{
// 		msg.m_code = ResultCode::FormatInvalid;
// 		msg.m_reason = "非法报文格式";
// 		msg.Build(true);
// 		host.Send(msg, msg.Size());
// 		return;
// 	}
// }
