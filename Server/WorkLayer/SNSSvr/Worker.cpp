#include "Worker.h"
#include "mdk/mapi.h"
#include "Interface/DBCenter/cpp/DBCenterCluster.h"
#include "Protocl/cpp/Object/Notify/Event.h"

#include "Protocl/cpp/Object/SNS/AddBuddy.h"
#include "Protocl/cpp/Object/SNS/DelBuddy.h"
#include "Protocl/cpp/Object/SNS/GetBuddys.h"
#include "Protocl/cpp/Object/SNS/Buddys.h"
#include "Protocl/cpp/Object/SNS/Chat.h"
#include "Protocl/cpp/Object/SNS/SetUserData.h"
#include "Protocl/cpp/Object/SNS/GetUserData.h"
#include "Protocl/cpp/Object/SNS/UserData.h"

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

	int notifyCount = 0;
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
	m_log.Info("Run", "DB服务器节点%d个", dbCenters.size());
	m_notifyCluster.SetNodeCount(notifyCount);
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
	else if (MsgId::delBuddy == msgId) OnDelBuddy(host, buffer);
	else if (MsgId::getBuddys == msgId) OnGetBuddys(host, buffer);
	else if (MsgId::chat == msgId) OnChat(host, buffer);
	else if (MsgId::setUserData == msgId) OnSetUserData(host, buffer);
	else if (MsgId::getUserData == msgId) OnGetUserData(host, buffer);
	else
	{
		std::string strIP;  // IP
		int         nPort;  // Port
		host.GetAddress(strIP, nPort);
		m_log.Info("Warning", "忽略报文!!! IP = %s, PORT = %d, msgId = %d", strIP.c_str(), nPort, msgId);
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

bool Worker::OnAddBuddy(mdk::NetHost& host, msg::Buffer &buffer)
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
	if ( 0 == msg.m_step )
	{
		std::map<mdk::uint32, mdk::uint32> buddyIds;
		if ( Redis::unsvr == m_cache.GetBuddys(msg.m_objectId, buddyIds) )
		{
			msg.m_code   = ResultCode::DBError;
			msg.m_reason = "Redis服务异常";
			msg.Build(true);
			host.Send(msg, msg.Size());
			m_log.Info("Error", "添加小伙伴失败：Redis服务异常");
			return true;
		}
		if ( buddyIds.end() != buddyIds.find(msg.m_buddyId) )
		{
			msg.m_code   = ResultCode::Refuse;
			msg.m_reason = "已经是小伙伴";
			msg.Build(true);
			host.Send(msg, msg.Size());
			m_log.Info("Error", "放弃添加小伙伴：已经是小伙伴");
			return true;
		}
	}

	Cache::User ui;
	ui.id = msg.m_objectId;
	if ( Redis::success != m_cache.GetUserInfo(ui) )
	{
		m_log.Info("Error", "添加小伙伴失败：读用户信息失败");
		msg.m_code   = ResultCode::DBError;
		msg.m_reason = "读用户信息失败";
		msg.Build(true);
		host.Send(msg, msg.Size());
		return true;
	}
	msg.m_userId = ui.id;
	msg.m_nickName = ui.nickName;
	msg.Build();
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
			msg.Build();
		}
	}
	if ( !SendNotify(msg.m_buddyId, msg::Event::user, &msg, -1, msg.m_userId, msg.m_nickName) )
	{
		m_log.Info("Error", "添加小伙伴失败：消息中心无可用服务");
	}

	return true;
}

bool Worker::OnDelBuddy(mdk::NetHost& host, msg::Buffer &buffer)
{
	msg::DelBuddy msg;
	memcpy(msg, buffer, buffer.Size());

	if ( !msg.Parse() )
	{
		msg.m_code   = ResultCode::FormatInvalid;
		msg.m_reason = "报文格式非法";
		msg.Build(true);
		host.Send(msg, msg.Size());
		return true;
	}
	msg.m_userId = msg.m_objectId;
	msg.Build();
	DBCenter *pDBCenter = ((DBCenterCluster*)SafeObject())->Node(msg.m_userId);
	pDBCenter->DelBuddy(msg);
	if ( ResultCode::Success == msg.m_code ) 
	{
		if ( !SendNotify(msg.m_buddyId, msg::Event::user, &msg, time(NULL) + 86400 * 7) )
		{
			m_log.Info("Error", "小伙伴已经删除，无法通知对方：消息中心无可用服务");
		}
	}

	msg.Build(true);
	host.Send(msg, msg.Size());

	return true;
}

bool Worker::OnGetBuddys(mdk::NetHost& host, msg::Buffer &buffer)
{
	msg::GetBuddys msg;
	memcpy(msg, buffer, buffer.Size());

	if ( !msg.Parse() )
	{
		msg.m_code   = ResultCode::FormatInvalid;
		msg.m_reason = "报文格式非法";
		msg.Build(true);
		host.Send(msg, msg.Size());
		return true;
	}
	msg.m_userId = msg.m_objectId;
	std::map<mdk::uint32, mdk::uint32> buddyIds;
	if ( Redis::unsvr == m_cache.GetBuddys(msg.m_userId, buddyIds) )
	{
		msg.m_code   = ResultCode::DBError;
		msg.m_reason = "Redis服务异常";
		msg.Build(true);
		host.Send(msg, msg.Size());
		return true;
	}

	//发送列表
	std::map<mdk::uint32, mdk::uint32>::iterator it;
	msg::Buddys buddyList;
	buddyList.m_connectId = msg.m_connectId;//连接Id(TCP接入服填写)
	buddyList.m_objectId = msg.m_objectId;//用户id(TCP接入服填写)
	memcpy(buddyList.m_ip, msg.m_ip, 4);//client ip
	buddyList.m_type = msg::Buddys::buddyList;
	buddyList.m_userId = msg.m_userId;
	msg::Buddys::BUDDY buddyItem;
	Cache::User ui;
	for ( it = buddyIds.begin(); it != buddyIds.end(); it++ )
	{
		buddyItem.id = it->second;
		ui.Clear();
		ui.id = buddyItem.id;
		buddyItem.nickName = "未获取";
		buddyItem.face = "---";
		if ( Redis::success == m_cache.GetUserInfo(ui) ) 
		{
			buddyItem.nickName = ui.nickName;
			buddyItem.face = ui.headIco;
		}
		buddyList.m_buddys.push_back(buddyItem);
		if ( 100 == buddyList.m_buddys.size() )
		{
			buddyList.Build();
			host.Send(buddyList, buddyList.Size());
			buddyList.m_buddys.clear();
		}
	}
	if ( 0 < buddyList.m_buddys.size() )
	{
		buddyList.Build();
		host.Send(buddyList, buddyList.Size());
		buddyList.m_buddys.clear();
	}

	return true;
}

bool Worker::OnChat(mdk::NetHost& host, msg::Buffer &buffer)
{
	msg::Chat msg;
	memcpy(msg, buffer, buffer.Size());

	if ( !msg.Parse() )
	{
		msg.m_code   = ResultCode::FormatInvalid;
		msg.m_reason = "报文格式非法";
		msg.Build(true);
		host.Send(msg, msg.Size());
		return true;
	}
	msg.m_senderId = msg.m_objectId;
	if ( msg::Chat::buddy == msg.m_recvType )
	{
		std::map<mdk::uint32,mdk::uint32> buddyIds;
		Redis::Result ret = m_cache.GetBuddys(msg.m_senderId, buddyIds);
		if ( Redis::unsvr == ret )
		{
			m_log.Info("Error", "访问伙伴列表失败");
			return false;
		}
		if ( Redis::nullData == ret || buddyIds.end() == buddyIds.find(msg.m_recverId) )
		{
			m_log.Info("Error", "不是伙伴不能发消息");
			msg.m_code = ResultCode::Refuse;
			msg.m_reason = "不是小伙伴";
			msg.Build(true);
			host.Send(msg, msg.Size());
			return false;
		}
	}
	else if ( msg::Chat::buddys == msg.m_recvType )
	{
		std::map<mdk::uint32,mdk::uint32> buddyIds;
		Redis::Result ret = m_cache.GetBuddys(msg.m_senderId, buddyIds);
		if ( Redis::unsvr == ret )
		{
			m_log.Info("Error", "访问伙伴列表失败");
			return false;
		}
		if ( Redis::nullData == ret ) return false;
	}
	else if ( msg::Chat::group == msg.m_recvType )
	{
		std::map<mdk::uint32,mdk::uint32> ids;
		Redis::Result ret = m_cache.GetGroupMember(msg.m_recverId, ids);
		if ( Redis::unsvr == ret )
		{
			m_log.Info("Error", "访问分组列表失败");
			return false;
		}
		if ( Redis::nullData == ret || ids.end() == ids.find(msg.m_senderId) ) 
		{
			m_log.Info("Error", "用户不在分组中，拒绝向该分组发送消息");
			return false;
		}
	}

	while ( false );
	Cache::User ui;
	ui.id = msg.m_senderId;
	if ( Redis::success != m_cache.GetUserInfo(ui) )
	{
		m_log.Info("Error", "聊天失败：读用户信息失败");
		msg.m_code   = ResultCode::DBError;
		msg.m_reason = "读用户信息失败";
		msg.Build(true);
		host.Send(msg, msg.Size());
		return true;
	}
	msg.m_senderName = ui.nickName;
	msg.m_senderFace = ui.headIco;
	msg.Build();
	SendNotify(msg.m_recverId, msg.m_recvType, &msg, time(NULL) + 86400 * 7, msg.m_senderId, msg.m_senderName);

	return true;
}

bool Worker::OnSetUserData(mdk::NetHost& host, msg::Buffer &buffer)
{
	msg::SetUserData msg;
	memcpy(msg, buffer, buffer.Size());

	if ( !msg.Parse() )
	{
		msg.m_code   = ResultCode::FormatInvalid;
		msg.m_reason = "报文格式非法";
		msg.Build(true);
		host.Send(msg, msg.Size());
		return true;
	}
	DBCenter *pDBCenter = ((DBCenterCluster*)SafeObject())->Node(msg.m_userId);
	pDBCenter->SetUserData(msg);
	return true;
}

bool Worker::OnGetUserData(mdk::NetHost& host, msg::Buffer &buffer)
{
	msg::GetUserData msg;
	memcpy(msg, buffer, buffer.Size());

	if ( !msg.Parse() )
	{
		msg.m_code   = ResultCode::FormatInvalid;
		msg.m_reason = "报文格式非法";
		msg.Build(true);
		host.Send(msg, msg.Size());
		return false;
	}
	if ( msg.m_objectId != msg.m_userId )//权限检查
	{
		std::map<mdk::uint32,mdk::uint32> ids;
		Redis::Result ret = m_cache.GetBuddys(msg.m_objectId, ids);
		if ( Redis::unsvr == ret )
		{
			m_log.Info("Error", "查询用户信息失败：无法获取用户伙伴列表");
			return false;
		}
		if ( Redis::nullData == ret )
		{
			msg.m_code   = ResultCode::Refuse;
			msg.m_reason = "不能查询陌生人信息";
			msg.Build(true);
			host.Send(msg, msg.Size());
			return false;
		}
	}
	Cache::User ui;
	ui.id = msg.m_userId;
	Redis::Result ret = m_cache.GetUserInfo(ui);
	if ( Redis::success != ret ) 
	{
		m_log.Info("Error", "查询用户信息失败：读缓存失败");
		return false;
	}
	msg::UserData data;
	data.m_objectId = msg.m_objectId;
	data.m_userId = ui.id;
	data.m_nickName = ui.nickName;
	data.m_sex = ui.sex;
	data.m_coin = ui.coin;
	data.m_face = ui.headIco;
	data.m_bindImei = ui.bindImei;
	data.m_bindMobile = ui.bindMobile;
	data.Build();
	host.Send(data, data.Size());

	return true;
}
