#include "Worker.h"
#include "mdk/mapi.h"
#include <cstring>
#include <cstdlib>
#include "common/common.h"
#include "Protocl/cpp/Object/ConnectAuth.h"
//认证模块消息
#include "Protocl/cpp/Object/Auth/UserRegister.h"
#include "Protocl/cpp/Object/Auth/UserLogin.h"
#include "Protocl/cpp/Object/Auth/UserLogout.h"
#include "Protocl/cpp/Object/Auth/ResetPassword.h"
#include "Protocl/cpp/Object/Auth/BindingPhone.h"
#include "Protocl/cpp/Object/Auth/UserRelogin.h"
#include "Protocl/cpp/Object/Notify/Event.h"

#ifdef WIN32
#ifdef _DEBUG
#pragma comment ( lib, "mdk_d.lib" )
#pragma comment ( lib, "hiredis_d.lib" )
#else
#pragma comment ( lib, "mdk.lib" )
#pragma comment ( lib, "hiredis.lib" )
#endif
#endif


static void DelFromVector(std::vector<mdk::NetHost> &src, mdk::NetHost &host)
{
	std::vector<mdk::NetHost>::iterator it = src.begin();
	for ( ; it != src.end(); it++ )
	{
		if ( it->ID() != host.ID() ) continue;
		src.erase(it);
		return;
	}

	return;
}

static void GetIP( mdk::NetHost &host, unsigned char *sip )
{
	std::string ip;
	int port;
	host.GetAddress(ip, port);
	sscanf(ip.c_str(), "%u.%u.%u.%u", &sip[0], &sip[1], &sip[2], &sip[3]);

	return;
}

Worker::Worker(void)
{
	m_log.SetLogName("TCPEntry");
	m_log.SetPrintLog(true);

	char exeDir[256];
	mdk::GetExeDir( exeDir, 256 );//取得可执行程序位置
	char cfgFile[256];
	sprintf( cfgFile, "%s/TCPEntry.cfg", exeDir );
	m_log.Info("Run", "读取配置：%s", cfgFile);
	if ( !m_cfg.ReadConfig( cfgFile ) )
	{
		m_log.Info( "Error", "配置错误" );
		mdk::mdk_assert(false);
		exit(0);
	}
	std::string ip = m_cfg["opt"]["ip"];
	int port = m_cfg["opt"]["listen"];
	m_cluster.SetService(m_cfg["ClusterMgr"]["ip"], m_cfg["ClusterMgr"]["port"]);
	msg::Cluster clusterInfo;
	std::string reason;
	m_log.Info( "Run", "集群配置服务(%s %d)", ((std::string)m_cfg["ClusterMgr"]["ip"]).c_str(), 
		(int)m_cfg["ClusterMgr"]["port"] );
	if ( ResultCode::success != m_cluster.GetCluster(Moudle::all, clusterInfo, reason) )
	{
		m_log.Info( "Error", "获取集群信息失败:%s", reason.c_str() );
		mdk::mdk_assert(false);
		exit(0);
	}
	m_authSvrCount = 0;
	m_snsSvrCount = 0;
	m_dbSvrCount = 0;
	int notifySvrCount = 0;
	ConnectInfo *pNode;
	std::map<Moudle::Moudle, std::map<NetLine::NetLine, std::vector<msg::Cluster::NODE> > >::iterator itMoudle;
	std::map<NetLine::NetLine, std::vector<msg::Cluster::NODE> >::iterator itLine;
	std::vector<msg::Cluster::NODE>::iterator it;
	for ( itMoudle = clusterInfo.m_cluster.begin(); itMoudle != clusterInfo.m_cluster.end(); itMoudle++ )
	{
		itLine = itMoudle->second.begin();
		for ( ; itLine != itMoudle->second.end(); itLine++ )
		{
			for ( it = itLine->second.begin(); it != itLine->second.end(); it++ )
			{
				if ( Moudle::TcpEntry == itMoudle->first )
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
				else if ( Moudle::Notify == itMoudle->first ) 
				{
					notifySvrCount++;
					continue;
				}
				else if ( Moudle::Auth == itMoudle->first ) m_authSvrCount++;
				else if ( Moudle::SNS == itMoudle->first ) m_snsSvrCount++;
				else if ( Moudle::DBEntry == itMoudle->first ) m_dbSvrCount++;
				else continue;

				pNode = new ConnectInfo;
				pNode->nodeId = it->nodeId;
				pNode->type = itMoudle->first;
				pNode->lineType = itLine->first;
				pNode->ip = it->ip;
				pNode->port = it->port;
				Connect(pNode->ip.c_str(), pNode->port, pNode, 5);
			}
		}
	}
	m_log.Info("Run", "认证结点%d个, sns结点%d个, DBEntry结点%d个", m_authSvrCount, m_snsSvrCount, m_dbSvrCount);
	if ( 0 >= m_authSvrCount || 0 >= m_snsSvrCount || 0 >= m_dbSvrCount )
	{
		m_log.Info( "Error", "集群缺少模块，请检查Cluster库" );
		mdk::mdk_assert(false);
		exit(0);
	}
	m_notifyCluster.SetNodeCount(notifySvrCount);
	m_authCluster.SetNodeCount(m_authSvrCount);
	m_snsCluster.SetNodeCount(m_snsSvrCount);
	m_dbCluster.SetNodeCount(m_dbSvrCount);
	int CPU = mdk::GetCUPNumber(32, 32);
	SetWorkThreadCount(CPU);
	m_cache.InitCluster(m_cfg, CPU);


	m_log.Info( "Run", "监听端口:%d", port );
	Listen(port);
}

Worker::~Worker(void)
{
}

void Worker::SaveConnect(mdk::NetHost &host)
{
	mdk::AutoLock lock(&m_lockClient);
	m_client[host.ID()] = host;

	return;
}


bool Worker::GetConnect(mdk::int64 connectId, mdk::NetHost &host)
{
	mdk::AutoLock lock(&m_lockClient);
	if ( m_client.find(connectId) == m_client.end() ) return false;
	host = m_client[connectId];

	return true;
}

bool Worker::DelConnect(mdk::int64 connectId)
{
	mdk::AutoLock lock(&m_lockClient);
	if ( m_client.find(connectId) == m_client.end() ) return false;
	m_client.erase(connectId);

	return true;
}

bool Worker::DelConnect(mdk::int64 connectId, mdk::NetHost &host)
{
	mdk::AutoLock lock(&m_lockClient);
	if ( m_client.find(connectId) == m_client.end() ) return false;
	host = m_client[connectId];
	m_client.erase(connectId);

	return true;
}

void Worker::OnConnect(mdk::NetHost &host)
{
	if ( !host.IsServer() )
	{
		std::string ip;
		int port;
		host.GetAddress(ip, port);
		m_log.Info("Run", "client(%s:%d):连接", ip.c_str(), port);
		SaveConnect(host);

		return;
	}

	ConnectInfo *pNode = (ConnectInfo*)host.GetSvrInfo();
	if ( Moudle::Auth == pNode->type )
	{
		m_authCluster.AddNode(pNode->nodeId, host);
		m_log.Info("Run", "认证服务(%d):%s:%d连接完成", pNode->nodeId, pNode->ip.c_str(), pNode->port);
	}
	if ( Moudle::SNS == pNode->type )
	{
		m_snsCluster.AddNode(pNode->nodeId, host);
		m_log.Info("Run", "SNS服务(%d):%s:%d连接完成", pNode->nodeId, pNode->ip.c_str(), pNode->port);
	}
	if ( Moudle::DBEntry == pNode->type )
	{
		m_dbCluster.AddNode(pNode->nodeId, host);
		m_log.Info("Run", "数据服务(%d):%s:%d连接完成", pNode->nodeId, pNode->ip.c_str(), pNode->port);
	}
	
}

void Worker::OnCloseConnect(mdk::NetHost &host)
{
	if ( !host.IsServer() )
	{
		ConnectInfo *pData = (ConnectInfo*)host.GetData();
		if ( NULL == pData ) 
		{
			std::string ip;
			int port;
			host.GetAddress(ip, port);
			m_log.Info("Error", "client(%s:%d)断开", ip.c_str(), port);
			DelConnect(host.ID());
			return;
		}
		if ( Moudle::Client == pData->type )
		{
			UserLogout(host);
		}
		else if ( Moudle::Notify == pData->type )
		{
			m_log.Info("Run", "Notify(%d):%s:%d连接断开", 
				pData->nodeId, pData->ip.c_str(), pData->port);
			m_notifyCluster.DelNode(pData->nodeId);
		}
		
		return;
	}

	ConnectInfo *pNode = (ConnectInfo*)host.GetSvrInfo();
	if ( Moudle::Auth == pNode->type )
	{
		m_authCluster.DelNode(pNode->nodeId);
		m_log.Info("Run", "认证服务(%d):%s:%d连接断开", pNode->nodeId, pNode->ip.c_str(), pNode->port);
	}
	if ( Moudle::SNS == pNode->type )
	{
		m_snsCluster.DelNode(pNode->nodeId);
		m_log.Info("Run", "SNS服务(%d):%s:%d连接断开", pNode->nodeId, pNode->ip.c_str(), pNode->port);
	}
	if ( Moudle::DBEntry == pNode->type )
	{
		m_dbCluster.DelNode(pNode->nodeId);
		m_log.Info("Run", "数据服务(%d):%s:%d连接断开", pNode->nodeId, pNode->ip.c_str(), pNode->port);
	}
	
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
	if ( Moudle::Notify == buffer.MoudleId() ) 
	{
		OnNotify(host, buffer);
		return;
	}

	if ( !buffer.IsResult() ) FillTCPParam(host, buffer);
	//按模块分发消息
	if ( Moudle::Auth == buffer.MoudleId() ) //认证模块特殊处理
	{
		OnAuth(host, buffer);
		return;
	}
	if ( host.IsServer() ) //业务层回应结果/主动推送数据
	{
		OnServerMsg( buffer );
		return;
	}

	//client请求
	if ( !buffer.Parse() )
	{
		host.Close();
		m_log.Info("Error","非法报文头，强制断开");
		return;
	}
	if ( Moudle::DBEntry == buffer.MoudleId() 
		&& (MsgId::setupVersion == buffer.Id())) 
	{
		mdk::NetHost helperHost;
		int nodeId = 0;
		if ( Moudle::DBEntry == buffer.MoudleId() ) nodeId = m_dbCluster.Node(helperHost);
		if ( 0 == nodeId )
		{
			m_log.Info("Error", "DBEntry结点未连接");
			return;
		}
		helperHost.Send(buffer, buffer.Size());
		return;
	}
	if ( buffer.IsResult() ) //不应该发送回应报文
	{
		host.Close();
		return;
	}
	if ( Moudle::all == buffer.MoudleId() ) 
	{
		OnAllMod(host, buffer);
		return;
	}

	//////////////////////////////////////////////////////////////////////////
	//业务指令
	User *pUser = (User*)host.GetData();
	if ( NULL == pUser || Moudle::Client != pUser->type ) //必须登录
	{
		m_log.Info("Error","请求需要登录，断开连接");
		host.Close();
		return;
	}
	mdk::NetHost helperHost;
	int nodeId = 0;
	if ( Moudle::SNS == buffer.MoudleId() ) nodeId = m_snsCluster.Node(helperHost);
	if ( Moudle::DBEntry == buffer.MoudleId() ) nodeId = m_dbCluster.Node(helperHost);
	if ( 0 == nodeId )
	{
		m_log.Info("Error", "服务结点未连接");
		return;
	}
	helperHost.Send(buffer, buffer.Size());

	return;
}

bool Worker::OnServerMsg( msg::Buffer &buffer )
{
	if ( !buffer.Parse() )
	{
		m_log.Info("Error", "非法格式报文");
		return false;
	}
	//回应消息
	mdk::NetHost clientHost;
	if (! GetUser(buffer.m_objectId, clientHost))
	{
		if ( !GetConnect(buffer.m_connectId, clientHost ) )
		{
			m_log.Info("Waring", "找不到Client连接，回应结果无法到达Client");
			return true;
		}
	}
	if ( ResultCode::msgError == buffer.m_code )
	{
		clientHost.Close();
		m_log.Info("Waring", "拒绝连接(%llu),原因(%d):%s", 
			buffer.m_connectId, buffer.m_code, buffer.m_reason.c_str() );
		return true;
	}
	clientHost.Send(buffer, buffer.Size());

	return true;
}

bool Worker::OnAllMod(mdk::NetHost &host, msg::Buffer &buffer)
{
	switch ( buffer.Id() )
	{
	case MsgId::connectAuth:
		return OnConnectAuth(host, buffer);
	default:
		return true;
	}

	return true;
}

bool Worker::OnConnectAuth(mdk::NetHost &host, msg::Buffer &buffer)
{
	msg::ConnectAuth msg;
	memcpy(msg, buffer, buffer.Size());
	if ( !msg.Parse() )
	{
		m_log.Info("Error", "解析连接认证报文失败");
		host.Close();
		return false;
	}
	ConnectInfo *pData = new ConnectInfo;
	pData->nodeId = msg.m_nodeId;
	pData->type = msg.m_nodeType;
	std::string ip;
	int port;
	host.GetAddress(pData->ip, pData->port);
	host.SetData(pData);
	const char *modName;
	if ( Moudle::Notify == pData->type ) 
	{
		modName = "Notify";
		m_notifyCluster.AddNode(pData->nodeId, host);
	}
	m_log.Info("Run", "%s(%d):%s:%d已登录", modName, pData->nodeId, pData->ip.c_str(), pData->port );
	DelConnect(host.ID());

	return true;
}

void Worker::FillTCPParam( mdk::NetHost &host, msg::Buffer &buffer )
{
	User *pUser = (User*)host.GetData();
	if ( NULL != pUser && Moudle::Client == pUser->type ) 
	{
		buffer.m_objectId = pUser->id;//用户id(TCP接入服填写)
	}
	buffer.m_connectId = host.ID();//连接Id(TCP接入服填写)
	std::string ip;
	int port;
	host.GetAddress(ip, port);
	sscanf(ip.c_str(), "%u.%u.%u.%u", &buffer.m_ip[0], &buffer.m_ip[1], &buffer.m_ip[2], &buffer.m_ip[3]);//client ip
	buffer.Build();
}

//Auth请求
bool Worker::OnAuth(mdk::NetHost &host, msg::Buffer &buffer)
{
	if ( MsgId::userLogin == buffer.Id() ) return OnUserLogin(host, buffer);//登录请求特殊处理
	if ( host.IsServer() ) return OnServerMsg( buffer );//业务层回应结果/主动推送数据
	if ( buffer.IsResult() ) //client只能发请求
	{
		host.Close();
		return false;
	}

	//不需要登录就可以执行的指令
	switch ( buffer.Id() )
	{
	case MsgId::userRegister :
		return OnUserRegister(host, buffer);
	default:
		break;
	}

	//需要登录才能执行的指令
	User *pUser = (User*)host.GetData();
	if ( NULL == pUser || Moudle::Client != pUser->type ) 
	{
		m_log.Info("Error","请求需要登录，断开连接");
		host.Close();
		return true;
	}
	switch ( buffer.Id() )
	{
	case MsgId::resetPassword:
	case MsgId::bindingPhone:
		{
			mdk::NetHost authHost;
			int nodeId = m_authCluster.Node(pUser->id, authHost);
			if ( 0 == nodeId )
			{
				m_log.Info("Error", "Auth结点未连接");
				return false;
			}
			authHost.Send(buffer, buffer.Size());
		}
		return true;
	default:
		return true;
	}
}

bool Worker::OnUserRegister(mdk::NetHost &host, msg::Buffer &buffer)
{
	msg::UserRegister msg;
	memcpy(msg, buffer, buffer.Size());
	if ( !msg.Parse() )
	{
		m_log.Info("Error", "解析用户注册报文失败");
		host.Close();
		return false;
	}
	mdk::NetHost auth;
	int nodeId = m_authCluster.Node(auth);
	if ( 0 == nodeId ) 
	{
		m_log.Info("Error", "无auth结点");
		return false;
	}
	auth.Send(msg, msg.Size());

	return true;
}

bool Worker::OnUserLogin(mdk::NetHost &host, msg::Buffer &buffer)
{
	if ( NULL != host.GetData() ) 
	{
		m_log.Info("Error", "已经认证连接不可以登录,强制断开");
		host.Close();
		return false;
	}
	msg::UserLogin msg;
	memcpy(msg, buffer, buffer.Size());
	if ( !msg.Parse() )
	{
		m_log.Info("Error", "解析用户登录报文失败");
		if ( !host.IsServer() ) host.Close();
		return false;
	}

	if ( !msg.IsResult() )//登录请求，执行转发
	{
		mdk::uint32 userId = 0;
		Redis::Result ret = m_cache.GetUserId(msg.m_accountType, msg.m_account, userId);
		if ( Redis::unsvr == ret )
		{
			msg.m_code = ResultCode::refuse;
			msg.m_reason = "Redis未连接";
			m_log.Info("Error", "登录失败：GetUserId，Redis未连接");
			msg.Build(true);
			host.Send(msg, msg.Size());
			return false;
		}
		if ( Redis::nullData == ret )
		{
			msg.m_code = ResultCode::refuse;
			msg.m_reason = "账号不存在";
			m_log.Info("Error", "账号(%s)不存在", msg.m_account.c_str());
			msg.Build(true);
			host.Send(msg, msg.Size());
			return false;
		}
		mdk::NetHost auth;
		int nodeId = m_authCluster.Node(userId, auth);
		if ( 0 == nodeId )
		{
			m_log.Info("Error", "Auth结点未连接");
			return false;
		}
		auth.Send(msg, msg.Size());

		return false;
	}
	//回应消息
	mdk::NetHost userHost;
	if ( 0 == msg.m_code )
	{
		if ( !DelConnect(msg.m_connectId, userHost) ) 
		{
			m_log.Info("Waring","请求方已断开");
			return false;
		}
		User *pUser = new User;
		pUser->type = Moudle::Client;
		userHost.GetAddress(pUser->ip, pUser->port);
		pUser->id = msg.m_userId;
		pUser->clientType = msg.m_clientType;
		pUser->accountType = msg.m_accountType;
		pUser->account = msg.m_account;
		userHost.SetData(pUser);
		AddUser(userHost);
		SetHostRecv(userHost, true);
	}
	else
	{
		if ( "已在其它地方登录" == msg.m_reason ) NotifyRelogin(msg);
		if ( !GetConnect(msg.m_connectId, userHost) ) 
		{
			m_log.Info("Waring","请求方已断开");
			return false;
		}
	}
	userHost.Send(buffer, buffer.Size());
	return true;
}

void Worker::NotifyRelogin(msg::UserLogin &msg)
{
	mdk::uint32 userId = 0;
	if ( Redis::success != m_cache.GetUserId(msg.m_accountType, msg.m_account, userId) )
	{
		m_log.Info("Error", "查询用户id失败，无法通知已登录用户(%u)下线", userId);
		return;
	}
	mdk::int32 tcpEntryId;
	Redis::Result ret = m_cache.GetUserNode(userId, tcpEntryId);
	if ( Redis::unsvr == ret )
	{
		m_log.Info("Error", "无法访问redis，无法将用户(%u)踢下线", userId);
		return;
	}
	if ( Redis::nullData == ret ) 
	{
		m_log.Info("Error", "用户(%u)已下线,放弃踢人动作", userId);
		return;
	}
	mdk::NetHost userHost;
	std::string position = "未知";
	if ( GetConnect(msg.m_connectId, userHost) ) 
	{
		std::string ip;
		int port;
		userHost.GetAddress(ip, port);
		char temp[64];
		sprintf(temp, "%s:%d", ip.c_str(), port);
		position = temp;
	}

	mdk::NetHost notifyHost;
	if ( 0 == m_notifyCluster.Node(userId, notifyHost) )
	{
		m_log.Info("Error", "消息中心未连接，放弃踢用户(%u)下线", userId);
		return;
	}

	msg::UserRelogin relogin;
	relogin.m_position = position;
	relogin.Build();

	msg::Event e;
	e.m_senderId = userId;
	e.m_recvType = msg::Event::user;
	e.m_recverId = userId;
	e.m_holdTime = time(NULL);
	memcpy(e.m_msg, relogin, relogin.Size());
	e.Build();
	notifyHost.Send(e, e.Size());

	return;
}

bool Worker::UserLogout(mdk::NetHost &host)
{
	ConnectInfo *pData = (ConnectInfo *)host.GetData();
	if ( NULL == pData || Moudle::Client != pData->type ) return false;
	host.Close();

	SetHostRecv(host, false);//消息中心停止向连接对应用户转发消息
	User *pUser = (User*)pData;
	m_log.Info("Run", "用户(%d):%s登出", pUser->id, pUser->account.c_str());
	DelUser(pUser->id);

	//转发登出消息到Auth
	mdk::NetHost auth;
	int nodeId = m_authCluster.Node(pUser->id, auth);
	if ( 0 == nodeId )
	{
		m_log.Info("Error", "Auth结点未连接");
		return false;
	}
	msg::UserLogout msg;
	FillTCPParam(host, msg);
	msg.m_clientType = pUser->clientType;
	msg.Build();
	auth.Send(msg, msg.Size());
	return true;
}

//设置host是否可以接收消息中心转发消息
void Worker::SetHostRecv(mdk::NetHost &host, bool enable)
{
	User *pUser = (User*)host.GetData();
	if ( NULL == pUser ) 
	{
		m_log.Info("Error", "client绑定数据丢失");
		return;
	}

	if ( enable ) 
	{
		if ( !m_cache.SetUserNode(pUser->id, m_nodeId) )
		{
			m_log.Info("Error", "保存用户所在接入结点失败");
			return;
		}
	}
	else  
	{
		if ( !m_cache.DelUserNode(pUser->id) )
		{
			m_log.Info("Error", "删除用户所在接入结点失败");
			return;
		}
	}
}

mdk::NetHost Worker::AddUser(mdk::NetHost &host)
{
	User *pUser = (User*)host.GetData();
	mdk::AutoLock lock(&m_lockUser);
	std::map<mdk::int32,mdk::NetHost>::iterator it = m_user.find(pUser->id);
	mdk::NetHost oldUser;
	if ( it != m_user.end() ) oldUser = it->second;
	m_user[pUser->id] = host;

	return oldUser;
}

bool Worker::DelUser( mdk::uint32 userId )
{
	mdk::AutoLock lock(&m_lockUser);
	std::map<mdk::int32,mdk::NetHost>::iterator iter = m_user.find(userId);
	if (iter == m_user.end()) return false;
	m_user.erase(iter);

	return true;
}

bool Worker::GetUser(mdk::uint32 userId, mdk::NetHost& host)
{
	mdk::AutoLock lock(&m_lockUser);

	std::map<mdk::int32,mdk::NetHost>::iterator iter = m_user.find(userId);
	if (iter == m_user.end())
	{
		return false;
	}
	else
	{
		host = iter->second;
		return true;
	}
}

bool Worker::OnNotify(mdk::NetHost &host, msg::Buffer &buffer)
{
	if ( MsgId::event == buffer.Id() ) 
	{
		if ( !buffer.Parse() ) 
		{
			m_log.Info("Error","通知报文:非法格式");
			return false;
		}
		return NotifyUser(buffer);
	}

	//转发到消息中心，或回应
	if ( !buffer.IsResult() ) FillTCPParam(host, buffer);

	if ( MsgId::getEvent == buffer.Id() )
	{
		if ( !buffer.Parse() ) 
		{
			m_log.Info("Error","读消息报文:非法格式");
			return false;
		}
		if ( !buffer.IsResult() )//转发
		{
			mdk::NetHost msgCenter;
			if ( 0 == m_notifyCluster.Node(buffer.m_objectId, msgCenter) )
			{
				m_log.Info("Error", "消息中心未连接");
				return false;
			}
			msgCenter.Send(buffer, buffer.Size());
		}
		else
		{
			mdk::NetHost clientHost;
			if ( !GetUser(buffer.m_objectId, clientHost) )
			{
				m_log.Info("Waring", "找不到用户(%d)连接，通知无法到达用户", buffer.m_objectId);
				return false;
			}

			std::vector<std::string> events;
			Redis::Result ret = m_cache.GetEvents(buffer.m_objectId, events);
			if ( Redis::unsvr == ret )
			{
				m_log.Info("Error", "redis未连接");
				return false;
			}
			if ( Redis::success != ret ) return true;
			int i = 0;
			time_t curTime = time(NULL);
			for ( i = 0; i < events.size(); i++ )
			{
				msg::Event data;
				memcpy(data, events[i].c_str(), events[i].size());
				if ( !data.Parse() ) 
				{
					m_log.Info("Error", "1条消息无法解析");
					continue;
				}
				memcpy(buffer, data.m_msg, data.m_msg.Size());
				if ( !buffer.Parse() ) 
				{
					m_log.Info("Error", "1条消息无法解析");
					continue;
				}
				clientHost.Send(buffer, buffer.Size());
			}
		}
	}

	return true;
}

bool Worker::NotifyUser(msg::Buffer &buffer)
{
	msg::Event msg;
	memcpy(msg, buffer, buffer.Size());
	if ( !msg.Parse() )
	{
		m_log.Info("Error", "非法格式通知");
		return false;
	}
	if ( msg::Event::user != msg.m_recvType ) 
	{
		m_log.Info("Error", "未预料的群发消息");
		return false;
	}
	mdk::NetHost clientHost;
	if ( !GetUser(msg.m_recverId, clientHost) )
	{
		m_log.Info("Waring", "找不到用户(%d)连接，通知无法到达用户", msg.m_recverId);
		return false;
	}
	memcpy(buffer, msg.m_msg, msg.m_msg.Size());
	if ( !buffer.Parse() )
	{
		m_log.Info("Error", "非法格式报文");
		return false;
	}
	clientHost.Send(buffer, buffer.Size());
	return true;
}
