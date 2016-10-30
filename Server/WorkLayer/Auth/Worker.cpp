#include "Worker.h"
#include "mdk/mapi.h"
#include "common/common.h"
#include "common/MD5Helper.h"
#include "Protocl/cpp/Object/Auth/UserLogout.h"
#include "Protocl/cpp/Object/Auth/ResetPassword.h"
#include "Protocl/cpp/Object/Auth/BindingPhone.h"
#include "Interface/DBCenter/cpp/DBCenterCluster.h"

#ifdef WIN32
#ifdef _DEBUG
#pragma comment(lib, "mdk_d.lib")
#pragma comment(lib, "hiredis_d.lib")
#else
#pragma comment(lib, "mdk.lib")
#pragma comment(lib, "hiredis.lib")
#endif // _DEBUG
#endif // WIN32

static std::string MD5(const std::string& strKey)
{
	static MD5Helper md5;
	return md5.HashString(strKey.c_str(), strKey.size());
}

/**
 * Worker
 */
Worker::Worker(void)
{
	// 设置Log相关
	m_log.SetLogName("Auth");
	m_log.SetPrintLog(true);

	// 读取配置文件
	const int nBufferSize      = 256;
	char pExeDir[nBufferSize]  = {0};
	char pCfgFile[nBufferSize] = {0};

	// 取得可执行程序的位置
	mdk::GetExeDir(pExeDir, nBufferSize);
	// 取得配置文件的位置
	sprintf(pCfgFile, "%s/Auth.cfg", pExeDir);

	if (! m_cfg.ReadConfig(pCfgFile))
	{
		m_log.Info("Error", "Auth.cfg 配置错误!");
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
				if (Moudle::Auth == itMoudle->first && curSvrPORT == itNode->port && curSvrIP == itNode->ip)
				{
					m_cfg["opt"]["nodeId"] = itNode->nodeId;
					m_cfg.Save();
				}
				// 创建所有的DB连接
				if (Moudle::DBEntry == itMoudle->first) dbCenters.push_back(*itNode);
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

void Worker::OnCloseConnect(mdk::NetHost &host)
{
	//下线所有该结点上的client
	mdk::AutoLock lock(&m_usersLock);
	std::map<mdk::uint64, std::map<mdk::uint32, Cache::LoginState> >::iterator it;
	it = m_users.find(host.ID());
	if ( it == m_users.end() ) return;
	it->second.clear();
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

	// 按照msgId进行消息处理，并过滤掉忽略的消息，记入日志
	int msgId = buffer.Id();
	if (MsgId::userRegister == msgId) OnUserRegister(host, buffer);
	else if (MsgId::userLogin == msgId) OnUserLogin(host, buffer);
	else if (MsgId::userLogout == msgId) OnUserLogout(host, buffer);
	else if (MsgId::resetPassword == msgId) OnUserResetPwd(host, buffer);
	else if (MsgId::bindingPhone == msgId) OnUserBindingPhone(host, buffer);
	else
	{
		std::string strIP;  // IP
		int         nPort;  // Port
		host.GetAddress(strIP, nPort);
		m_log.Info("Warning", "忽略报文!!! IP = %s, PORT = %d, msgId = %d", strIP.c_str(), nPort, msgId);
	}
}

bool Worker::OnUserRegister(mdk::NetHost& host, msg::Buffer& buffer)
{
	msg::UserRegister msg;
	memcpy(msg, buffer, buffer.Size());

	do 
	{
		if ( !msg.Parse() )
		{
			msg.m_code   = ResultCode::FormatInvalid;
			msg.m_reason = "报文格式非法";
			break;
		}
		// 注册用户
		DBCenter *pDBCenter = ((DBCenterCluster*)SafeObject())->Node(msg.m_account);
		pDBCenter->CreateUser(msg); 
	}
	while (false);
	if ( ResultCode::Success != msg.m_code )
	{
		m_log.Info("Error", "注册失败：%s", msg.m_reason.c_str());
	}
	msg.Build(true);
	host.Send(msg, msg.Size());

	return (msg.m_code ? false : true);
}

bool Worker::OnUserLogin(mdk::NetHost& host, msg::Buffer& buffer)
{
	msg::UserLogin msg;
	memcpy(msg, buffer, buffer.Size());

	do 
	{
		if ( !msg.Parse() )
		{
			msg.m_code   = ResultCode::FormatInvalid;
			msg.m_reason = "报文格式非法";
			break;
		}
		//取userId如果取不出来，则表示账号不存在
		mdk::uint32 userId = 0;
		if ( Redis::success != m_cache.GetUserId(msg.m_accountType, msg.m_account, userId) )
		{
			msg.m_code = ResultCode::ResultCode(ErrorCode::EC_AccountNotExist);
			msg.m_reason = "获取用户id失败！";
			break;
		}
		{//检查重复登录
			bool logined = false;
			mdk::AutoLock lock(&m_usersLock);
			std::map<mdk::uint64, std::map<mdk::uint32, Cache::LoginState> >::iterator it;
			for ( it = m_users.begin(); it != m_users.end(); it++)
			{
				if ( it->second.end() != it->second.find(userId) )
				{
					Cache::LoginState &loginState = it->second[userId];
					if ( 
						(ClientType::android == msg.m_clientType && loginState.androidOnline)
						|| (ClientType::iphone == msg.m_clientType && loginState.androidOnline)
						) 
					{
						logined = true;
					}
					break;
				}
			}
			if ( logined )
			{
				msg.m_code = ResultCode::Refuse;
				msg.m_reason = "已在其它地方登录";
				break;
			}
		}

		//取出用户信息
		Cache::User userInfo;
		userInfo.id = userId;
		if ( Redis::success != m_cache.GetUserInfo(userInfo))
		{
			msg.m_code = ResultCode::DBError;
			msg.m_reason = "获取用户信息失败！";
			break;
		}
		// 核对密码
		if (userInfo.pwd != MD5(msg.m_pwd + userInfo.randKey))
		{
			msg.m_code = ResultCode::ResultCode(ErrorCode::EC_PwdError);
			msg.m_reason = "密码错误！";
			break;
		}

		// 设置登陆状态
		if ( !SetLoginState(host.ID(), userId, msg.m_clientType, true) )
		{
			msg.m_code = ResultCode::DBError;
			msg.m_reason = "保存登录状态失败！";
			break;
		}

		// 成功，填充回复消息
		msg.m_userId = userId;
		msg.m_nick = userInfo.nickName;
	}
	while (false);
	if ( ResultCode::Success != msg.m_code )
	{
		m_log.Info("Error", "登录失败：%s", msg.m_reason.c_str());
	}
	msg.Build(true);
	host.Send(msg, msg.Size());

	return msg.m_code?false:true;
}

bool Worker::SetLoginState( mdk::uint64 tcpEntryId, mdk::uint32 userId, ClientType::ClientType type, bool online)
{
	mdk::AutoLock lock(&m_usersLock);
	if ( m_users[tcpEntryId].end() == m_users[tcpEntryId].find(userId) )
	{
		Cache::LoginState state;
		state.Build();
		m_users[tcpEntryId].insert(std::map<mdk::uint32, Cache::LoginState>::value_type(userId, state));        
	}
	Cache::LoginState &state = m_users[tcpEntryId][userId];
	if (ClientType::android == type) state.androidOnline = online;
	else if (ClientType::iphone == type) state.iphoneOnline =  online;

	if (!m_cache.SetLoginState(userId, state)) 
	{
		if (ClientType::android == type) state.androidOnline = false;
		else if (ClientType::iphone == type) state.iphoneOnline = false;
		return false;
	}

	return true;
}

bool Worker::OnUserLogout(mdk::NetHost& host, msg::Buffer& buffer)
{
	msg::UserLogout msg;
	memcpy(msg, buffer, buffer.Size());

	if ( !msg.Parse() )
	{
		m_log.Info("Error", "报文格式非法，断开连接");
		host.Close();
		return true;
	}
	// 重置登陆状态
	if ( !SetLoginState(host.ID(), msg.m_objectId, msg.m_clientType, false) )
	{
		m_log.Info("Waring", "Logout失败：保存状态失败！");
	}

	return true;
}

bool Worker::OnUserResetPwd(mdk::NetHost& host, msg::Buffer& buffer)
{
	msg::ResetPassword msg;
	memcpy(msg, buffer, buffer.Size());

	do 
	{
		if ( !msg.Parse() )
		{
			msg.m_code   = ResultCode::FormatInvalid;
			msg.m_reason = "报文格式非法";
			break;
		}
		// 再修改数据库中的密码
		DBCenter *pDBCenter = ((DBCenterCluster*)SafeObject())->Node(msg.m_objectId);
		pDBCenter->ResetUserPwd(msg);
	} 
	while (false);
	if ( ResultCode::Success != msg.m_code )
	{
		m_log.Info("Error", "修改密码失败：%s", msg.m_reason.c_str());
	}

	msg.Build(true);
	host.Send(msg, msg.Size());

	return (msg.m_code ? false : true);
}

bool Worker::OnUserBindingPhone(mdk::NetHost& host, msg::Buffer& buffer)
{
	msg::BindingPhone msg;
	memcpy(msg, buffer, buffer.Size());

	do 
	{
		if ( !msg.Parse() )
		{
			msg.m_code   = ResultCode::FormatInvalid;
			msg.m_reason = "报文格式非法";
			break;
		}
		// 再修改数据库中的用户信息
		DBCenter *pDBCenter = ((DBCenterCluster*)SafeObject())->Node(msg.m_objectId);
		pDBCenter->BindingPhone(msg);
	} 
	while (false);
	if ( ResultCode::Success != msg.m_code )
	{
		m_log.Info("Error", "绑定手机失败：%s", msg.m_reason.c_str());
	}

	msg.Build(true);
	host.Send(msg, msg.Size());

	return (msg.m_code ? false : true);
}
