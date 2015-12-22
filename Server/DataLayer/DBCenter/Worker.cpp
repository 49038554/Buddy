#include <cstdio>
#include <ctime>
#include <cstdlib>
#include <cstring>
#include "Worker.h"
#include "common/MD5Helper.h"
#include "mdk/mapi.h"
#include "Protocl/cpp/Object/Auth/ResetPassword.h"
#include "Protocl/cpp/Object/Auth/BindingPhone.h"

static MD5Helper gs_md5helper;

/**
 * Worker
 */
Worker::Worker(void)
{
	// 设置Log相关
	m_log.SetLogName("DBCenter");
	m_log.SetPrintLog(true);

	// 读取配置文件
	const int nBufferSize      = 256;
	char pExeDir[nBufferSize]  = {0};
	char pCfgFile[nBufferSize] = {0};

	// 取得可执行程序的位置
	mdk::GetExeDir(pExeDir, nBufferSize);
	// 取得配置文件的位置
	sprintf(pCfgFile, "%s/DBCenter.cfg", pExeDir);
	if (! m_cfg.ReadConfig(pCfgFile))
	{
		m_log.Info("Error", "DBCenter.cfg 配置错误!");
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
				if (Moudle::DBEntry == itMoudle->first && curSvrPORT == itNode->port && curSvrIP == itNode->ip)
				{
					m_cfg["opt"]["nodeId"] = m_nodeId = itNode->nodeId;
					m_cfg["Only Id"]["nodeId"] = m_nodeId = itNode->nodeId;
					m_cfg.Save();
				}
				else
				{
					continue;
				}
			}
		}
	}

	// 初始化数据库集群信息
	if ( !m_mySQLCluster.Init(m_cfg))
	{
		m_log.Info("Error", "初始化数据库集群失败！");
		mdk::mdk_assert(false);
		exit(EXIT_FAILURE);
	}

	// 初始化Redis集群
	if ( !m_cache.InitCluster(m_cfg, 1))
	{
		m_log.Info("Error", "Redis集群初始化失败！");
		mdk::mdk_assert(false);
		exit(EXIT_FAILURE);
	}

	// 播随机种子
	srand(unsigned(time(NULL)));

	InitId("DBCenter.cfg");
	// 监听端口，等待连接
	m_log.Info("Run", "监听端口:%d", curSvrPORT);
	Listen(curSvrPORT);
}

Worker::~Worker(void)
{
	// body
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

void Worker::OnMsg(mdk::STNetHost& host)
{
	msg::Buffer buffer;
	if (!host.Recv(buffer, buffer.HeaderSize(), false)) return;
	if (-1 == buffer.Size())
	{
		m_log.Info("Error", "非法报文长度! 强制断开");
		host.Close();
		return;
	}
	if (! host.Recv(buffer, buffer.Size())) return;
	if ( buffer.IsResult() )
	{
		m_log.Info("Error", "未预料的结果报文! 强制断开");
		host.Close();
		return;
	}

	switch (buffer.MoudleId())
	{
	case Moudle::Auth:    // 认证模块
		OnAuth(host, buffer);
		break;
	case Moudle::SNS :
		OnSNS(host, buffer);
		break;
	default:
		m_log.Info("Error", "未预料的报文! 强制断开");
		host.Close();
		break;
	}
}

void Worker::OnAuth(mdk::STNetHost &host, msg::Buffer &buffer)
{
	if (MsgId::userRegister == buffer.Id()) OnUserRegister(host, buffer);
	else if (MsgId::resetPassword == buffer.Id()) OnResetPassword(host, buffer);
	else if (MsgId::bindingPhone == buffer.Id()) OnBindingPhone(host, buffer);
}

bool Worker::OnUserRegister(mdk::STNetHost& host, msg::Buffer& buffer)
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
		mdk::uint32 userId;
		Redis::Result ret = m_cache.GetUserId(msg.m_accountType, msg.m_account, userId);
		if ( Redis::unsvr == ret )
		{
			msg.m_code   = ResultCode::ResultCode(ErrorCode::EC_UserExist);
			msg.m_reason = "Redis服务异常";
			break;
		}
		if (  Redis::success == ret )
		{
			msg.m_code   = ResultCode::ResultCode(ErrorCode::EC_UserExist);
			msg.m_reason = "账号已存在！";
			break;
		}
		Cache::User user;
		if ( !CreateUser(msg, user) ) break;
		msg.m_userId = user.id;
	} 
	while (false);
	if ( msg.m_code )
	{
		m_log.Info("Error", "注册失败:%s!", msg.m_reason.c_str());
	}
	msg.Build(true);
	host.Send(msg, msg.Size());

	return true;
}

bool Worker::OnResetPassword(mdk::STNetHost& host, msg::Buffer& buffer)
{
	msg::ResetPassword msg;
	memcpy(msg, buffer, buffer.Size());

	do 
	{
		if ( !msg.Parse() )
		{
			msg.m_code   = ResultCode::FormatInvalid;
			msg.m_reason = "报文格式非法";
			m_log.Info("Error", "报文格式非法!");
			break;
		}
		Cache::User ui;
		ui.id = msg.m_objectId;
		if ( Redis::success != m_cache.GetUserInfo(ui) )
		{
			msg.m_code   = ResultCode::DBError;
			msg.m_reason = "读Redis失败";
			break;
		}
		std::string pwd = msg.m_pwd + ui.randKey;
		ui.pwd = gs_md5helper.HashString(pwd.c_str(), pwd.size());
		MySqlClient* pMysql = m_mySQLCluster.Node("Buddy", msg.m_objectId);
		if (!pMysql)
		{
			msg.m_code   = ResultCode::DBError;
			msg.m_reason = "找不到Buddy数据库结点";
			break;
		}

		char sql[512]  = {0};
		sprintf(sql, "update user_info set pwd = \"%s\" where id = %d", 
			ui.pwd.c_str(), ui.id);
		if (!pMysql->ExecuteSql(sql))
		{
			msg.m_code   = ResultCode::DBError;
			msg.m_reason = "修改用户信息失败:";
			msg.m_reason += pMysql->GetLastError();
			break;
		}
		if ( !m_cache.SetUserInfo(ui) )
		{
			msg.m_code   = ResultCode::DBError;
			msg.m_reason = "写缓存失败";
			break;
		}
	}
	while (false);
	if ( ResultCode::Success != msg.m_code )
	{
		m_log.Info("Error", "修改密码失败：%s", msg.m_reason.c_str());
	}
	msg.Build(true);
	host.Send(msg, msg.Size());

	return true;
}

bool Worker::OnBindingPhone(mdk::STNetHost& host, msg::Buffer& buffer)
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
		// 然后把手机号更新至账号信息中
		MySqlClient *pMysql = m_mySQLCluster.Node("Auth", msg.m_phone);
		if (!pMysql)
		{
			msg.m_code   = ResultCode::DBError;
			msg.m_reason = "找不到Auth数据库结点";
			break;
		}
		mdk::uint32 userId;
		char sql[256];
		sprintf(sql, "select * from bind_mobile where id = %d", msg.m_objectId);
		if ( !pMysql->ExecuteSql(sql) )
		{
			msg.m_code   = ResultCode::DBError;
			msg.m_reason = "查询Auth库失败";
			msg.m_reason += pMysql->GetLastError();
			break;
		}
		if ( pMysql->IsEmpty() )
		{
			sprintf(sql, "insert into bind_mobile (mobile, id) values (\"%s\", %d)",
				msg.m_phone.c_str(), msg.m_objectId);
		}
		else
		{
			sprintf(sql, "update bind_mobile set mobile = \"%s\" where id = %d", 
				msg.m_phone.c_str(), msg.m_objectId);
		}
		if ( !pMysql->ExecuteSql(sql) )
		{
			msg.m_code   = ResultCode::DBError;
			msg.m_reason = "DB Error!";
			msg.m_reason += pMysql->GetLastError();
			break;
		}
		if ( !m_cache.BindUserName(AccountType::mobile, msg.m_phone, msg.m_objectId) )
		{
			msg.m_code   = ResultCode::DBError;
			msg.m_reason = "缓存账号信息失败";
			msg.m_reason += pMysql->GetLastError();
			break;
		}
	}
	while (false);
	if ( ResultCode::Success != msg.m_code )
	{
		m_log.Info("Error", "绑定手机失败：%s", msg.m_reason.c_str());
	}
	msg.Build(true);
	host.Send(msg, msg.Size());

	return true;
}

bool Worker::CreateUser(msg::UserRegister& userRegister, Cache::User& user)
{
	//////////////////////////////////////////////////////////////////////////
	//创建数据
	if (! CreateId(user.id)) 
	{
		userRegister.m_code   = ResultCode::Refuse;
		userRegister.m_reason = "Id资源用完！";
		return false;
	}
	RandNickname(user.nickName, user.id);
	RandKey(user.randKey);
	// 创建密码 -> MD5(userRegister.m_pwd + randKey)
	std::string pwd = userRegister.m_pwd + user.randKey;
	user.pwd = gs_md5helper.HashString(pwd.c_str(), pwd.size());

	//////////////////////////////////////////////////////////////////////////
	//写数据库
	MySqlClient* pMysql = m_mySQLCluster.Node("Auth", userRegister.m_account);
	if ( !pMysql ) 
	{
		userRegister.m_code   = ResultCode::DBError;
		userRegister.m_reason = "找不到Auth数据库结点";
		return false;
	}
	char sql[256] = {0};
	switch (userRegister.m_accountType)
	{
	case AccountType::imei :
		user.bindImei = userRegister.m_account;
		sprintf(sql, "insert into bind_imei (imei, id) values (\"%s\", %d)", 
			userRegister.m_account.c_str(), user.id);
		break;
	case AccountType::mobile:
		user.bindMobile = userRegister.m_account;
		sprintf(sql, "insert into bind_mobile (mobile, id) values (\"%s\", %d)", 
			userRegister.m_account.c_str(), user.id);
		break;
	default:
		userRegister.m_code   = ResultCode::Refuse;
		userRegister.m_reason = "非法账号类型";
		return false;
	}
	if (! pMysql->ExecuteSql(sql))
	{
		userRegister.m_code   = ResultCode::DBError;
		userRegister.m_reason = "写Auth库失败：";
		userRegister.m_reason += pMysql->GetLastError();
		return false;
	}
	pMysql = m_mySQLCluster.Node("Buddy", user.id);
	if ( !pMysql ) 
	{
		userRegister.m_code = ResultCode::DBError;
		userRegister.m_reason = "找不到Buddy数据库结点";
		return false;
	}
	sprintf(sql, "insert into user_info (id, pwd, randKey, nickname) "
				"values (%d, \"%s\", \"%s\", \"%s\")", 
				user.id, user.pwd.c_str(), user.randKey.c_str(), user.nickName.c_str());
	if (! pMysql->ExecuteSql(sql))
	{
		userRegister.m_code   = ResultCode::DBError;
		userRegister.m_reason = "写Buddy库失败：";
		userRegister.m_reason += pMysql->GetLastError();
		return false;
	}
	//////////////////////////////////////////////////////////////////////////
	//修改redis
	if ( !m_cache.SetUserInfo(user) )
	{
		userRegister.m_code   = ResultCode::DBError;
		userRegister.m_reason = "写Redis失败";
		return false;
	}
	if ( !m_cache.BindUserName(userRegister.m_accountType, userRegister.m_account, user.id) )
	{
		userRegister.m_code   = ResultCode::DBError;
		userRegister.m_reason = "写Redis失败";
		return false;
	}

	return true;
}

void Worker::RandNickname(std::string& nickname, int userId)
{
	// 昵称生成规则：userId个位数所对应的称呼 + 用户ID
	static const char* nicknames[]  = { // 所有的男性昵称
		"叶茹瑶"/* 0 */, "夜乳摇"/* 1 */, "凤玉娇"/* 2 */, "混元太极金仙"/* 3 */, "混沌无极金仙"/* 4 */,
		"怒仙邪"/* 5 */, "姬美丽"/* 6 */, "混元大罗金仙"/* 7 */, "飘花淡雪浮香吹"/* 8 */, "太上无极混元教主"/* 9 */
	};

	char pBuffer[128] = {0};

	sprintf(pBuffer, "%s%d", nicknames[userId % 10], userId);

	nickname = pBuffer;

	return;
}

void Worker::RandKey(std::string& randKey)
{
	char pBuffer[16]    = {0};
	const int nKeyLen   = 32;
	int nNumber         = 0;
	randKey             = "";
	for (int idx = 0; idx != nKeyLen; ++idx)
	{
		nNumber = rand() % 10;
		nNumber = ((0 == idx) && (0 == nNumber) ? 1 : nNumber);
		sprintf(pBuffer, "%d", nNumber);
		randKey += pBuffer;
	}

	return;
}

void Worker::OnSNS(mdk::STNetHost &host, msg::Buffer &buffer)
{
	if (MsgId::addBuddy == buffer.Id()) OnAddBuddy(host, buffer);
	if (MsgId::delBuddy == buffer.Id()) OnDelBuddy(host, buffer);
	if (MsgId::setUserData == buffer.Id()) OnSetUserData(host, buffer);
}

void Worker::OnAddBuddy(mdk::STNetHost &host, msg::Buffer &buffer)
{
	msg::AddBuddy msg;
	memcpy(msg, buffer, buffer.Size());

	do 
	{
		if ( !msg.Parse() )
		{
			msg.m_code   = ResultCode::FormatInvalid;
			msg.m_reason = "报文格式非法";
			break;
		}
		if ( !SetBuddy(msg.m_userId, msg.m_buddyId, msg) ) break;
		SetBuddy(msg.m_buddyId, msg.m_userId, msg);
	}
	while ( false );
	if ( msg.m_code )
	{
		m_log.Info("Error", "添加小伙伴失败:%s!", msg.m_reason.c_str());
	}
	msg.Build(true);
	host.Send(msg, msg.Size());
}

bool Worker::SetBuddy(mdk::uint32 userId, mdk::uint32 buddyId, msg::AddBuddy &msg)
{
	std::map<mdk::uint32, mdk::uint32> buddyIds;
	if ( Redis::unsvr == m_cache.GetBuddys(userId, buddyIds) )
	{
		msg.m_code   = ResultCode::DBError;
		msg.m_reason = "Redis服务异常";
		return false;
	}
	if ( buddyIds.end() != buddyIds.find(buddyId) )
	{
		msg.m_code   = ResultCode::Refuse;
		msg.m_reason = "已经是小伙伴";
		return true;
	}
	char sql[256];
	sprintf( sql, "insert into buddy (userId, buddyId) values(%u, %u)", 
		userId, buddyId );
	MySqlClient *pMysql = m_mySQLCluster.Node("Buddy", userId);
	if (!pMysql)
	{
		msg.m_code   = ResultCode::DBError;
		msg.m_reason = "找不到Buddy数据库结点";
		return false;
	}
	if ( !pMysql->ExecuteSql(sql) )
	{
		msg.m_code   = ResultCode::DBError;
		msg.m_reason = "修改数据库失败:";
		msg.m_reason += pMysql->GetLastError();
		return false;
	}
	if ( !m_cache.AddBuddy(userId, buddyId) )
	{
		msg.m_code   = ResultCode::DBError;
		msg.m_reason = "写缓存失败";
		return false;
	}
	return true;
}

void Worker::OnDelBuddy(mdk::STNetHost &host, msg::Buffer &buffer)
{
	msg::DelBuddy msg;
	memcpy(msg, buffer, buffer.Size());

	do 
	{
		if ( !msg.Parse() )
		{
			msg.m_code   = ResultCode::FormatInvalid;
			msg.m_reason = "报文格式非法";
			break;
		}
		if ( !DelBuddy(msg.m_buddyId, msg.m_userId, msg) ) break;
		DelBuddy(msg.m_userId, msg.m_buddyId, msg);
	}
	while ( false );
	if ( msg.m_code )
	{
		m_log.Info("Error", "删除小伙伴失败:%s!", msg.m_reason.c_str());
	}
	msg.Build(true);
	host.Send(msg, msg.Size());
}

bool Worker::DelBuddy(mdk::uint32 userId, mdk::uint32 buddyId, msg::DelBuddy &msg)
{
	char sql[256];
	sprintf( sql, "delete from buddy where userId = %u and buddyId = %u", 
		userId, buddyId );
	MySqlClient *pMysql = m_mySQLCluster.Node("Buddy", userId);
	if (!pMysql)
	{
		msg.m_code   = ResultCode::DBError;
		msg.m_reason = "找不到Buddy数据库结点";
		return false;
	}
	if ( !pMysql->ExecuteSql(sql) )
	{
		msg.m_code   = ResultCode::DBError;
		msg.m_reason = "修改数据库失败:";
		msg.m_reason += pMysql->GetLastError();
		return false;
	}
	if ( !m_cache.DelBuddy(userId, buddyId) )
	{
		msg.m_code   = ResultCode::DBError;
		msg.m_reason = "写缓存失败";
		return false;
	}
	return true;
}

void Worker::OnSetUserData(mdk::STNetHost &host, msg::Buffer &buffer)
{
	msg::SetUserData msg;
	memcpy(msg, buffer, buffer.Size());

	do 
	{
		if ( !msg.Parse() )
		{
			msg.m_code   = ResultCode::FormatInvalid;
			msg.m_reason = "报文格式非法";
			break;
		}
		SetUserData(msg);
	}
	while ( false );
	if ( msg.m_code )
	{
		m_log.Info("Error", "设置用户信息失败:%s!", msg.m_reason.c_str());
	}
	msg.Build(true);
	host.Send(msg, msg.Size());
}

bool Worker::SetUserData(msg::SetUserData &msg)
{
	Cache::User ui;
	ui.id = msg.m_userId;
	Redis::Result ret = m_cache.GetUserInfo(ui);
	if ( Redis::unsvr == ret )
	{
		msg.m_code   = ResultCode::SvrError;
		msg.m_reason = "redis无法访问";
		return false;
	}
	if ( Redis::nullData == ret )
	{
		msg.m_code   = ResultCode::DBError;
		msg.m_reason = "用户数据不存在";
		return false;
	}

	char sql[256];
	sprintf( sql, "update user_info set "  );
	bool frist = true;
	if ( msg.m_pwdOpt ) 
	{
		sprintf( sql, "%spwd = \'%s\'", sql, frist?" ":", ", msg.m_pwd.c_str() );
		frist = false;
		ui.pwd = msg.m_pwd;
	}
	if ( msg.m_nickNameOpt ) 
	{
		sprintf( sql, "%s%snickname = \'%s\'", sql, frist?" ":", ", msg.m_nickName.c_str() );
		frist = false;
		ui.nickName = msg.m_nickName;
	}
	if ( msg.m_faceOpt ) 
	{
		sprintf( sql, "%s%sheadIco = \'%s\'", sql, frist?" ":", ", msg.m_face.c_str() );
		frist = false;
		ui.headIco = msg.m_face;
	}
	if ( msg.m_sexOpt ) 
	{
		sprintf( sql, "%s%ssex = \'%d\'", sql, frist?" ":", ", msg.m_sex?1:0 );
		frist = false;
		ui.sex = msg.m_sex;
	}
	if ( msg.m_coinOpt ) 
	{
		sprintf( sql, "%s%scoin = \'%d\'", sql, frist?" ":", ", msg.m_coin );
		frist = false;
		ui.coin = msg.m_coin;
	}
	if ( msg.m_bindImeiOpt ) ui.bindImei = msg.m_bindImei;
	if ( msg.m_bindMobileOpt ) ui.bindMobile = msg.m_bindMobile;
	sprintf( sql, "%s where id = %u", sql, msg.m_userId );
	MySqlClient *pMysql = m_mySQLCluster.Node("Buddy", msg.m_userId);
	if (!pMysql)
	{
		msg.m_code   = ResultCode::DBError;
		msg.m_reason = "找不到Buddy数据库结点";
		return false;
	}
	if ( !pMysql->ExecuteSql(sql) )
	{
		msg.m_code   = ResultCode::DBError;
		msg.m_reason = "修改数据库失败:";
		msg.m_reason += pMysql->GetLastError();
		return false;
	}
	if ( !m_cache.SetUserInfo(ui) )
	{
		msg.m_code   = ResultCode::DBError;
		msg.m_reason = "写缓存失败";
		return false;
	}
	return true;
}

