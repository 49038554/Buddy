#include <cstdio>
#include <ctime>
#include <cstdlib>
#include <cstring>
#include "Worker.h"
#include "common/MD5Helper.h"
#include "mdk/mapi.h"
#include "Protocl/cpp/Object/Auth/ResetPassword.h"
#include "Protocl/cpp/Object/Auth/BindingPhone.h"
#include "Protocl/cpp/Object/Game/BuildHouse.h"
#include "Protocl/cpp/Object/Game/TreePlant.h"
#include "Protocl/cpp/Object/Game/SyncPets.h"
#include "Protocl/cpp/Object/Game/SyncItem.h"
#include "Protocl/cpp/Object/Game/SyncCoin.h"




static MD5Helper gs_md5helper;

/**
 * Worker
 */
Worker::Worker(void)
{
	// ����Log���
	m_log.SetLogName("DBCenter");
	m_log.SetPrintLog(true);

	// ��ȡ�����ļ�
	const int nBufferSize      = 256;
	char pExeDir[nBufferSize]  = {0};
	char pCfgFile[nBufferSize] = {0};

	// ȡ�ÿ�ִ�г����λ��
	mdk::GetExeDir(pExeDir, nBufferSize);
	// ȡ�������ļ���λ��
	sprintf(pCfgFile, "%s/DBCenter.cfg", pExeDir);
	if (! m_cfg.ReadConfig(pCfgFile))
	{
		m_log.Info("Error", "DBCenter.cfg ���ô���!");
		mdk::mdk_assert(false);
		exit(0);
	}
	std::string curSvrIP   = m_cfg["opt"]["ip"];
	int         curSvrPORT = m_cfg["opt"]["listen"];

	// �Ӽ�Ⱥ���÷���ȡ��Ⱥ������Ϣ
	m_cluster.SetSvr(m_cfg["ClusterMgr"]["ip"], m_cfg["ClusterMgr"]["port"]);
	m_log.Info("Run", "��Ⱥ���÷���(%s %d)", std::string(m_cfg["ClusterMgr"]["ip"]).c_str(), int(m_cfg["ClusterMgr"]["port"]));

	msg::Cluster clusterInfo;
	std::string  reason;
	if (SyncClient::SUCESS != m_cluster.GetCluster(Moudle::all, clusterInfo, reason))
	{
		m_log.Info( "Error", "��ȡ��Ⱥ��Ϣʧ��:%s", reason.c_str() );
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
				// ���ݼ�Ⱥ��Ϣ�������������Ľڵ�ID
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

	// ��ʼ�����ݿ⼯Ⱥ��Ϣ
	if ( !m_mySQLCluster.Init(m_cfg))
	{
		m_log.Info("Error", "��ʼ�����ݿ⼯Ⱥʧ�ܣ�");
		mdk::mdk_assert(false);
		exit(EXIT_FAILURE);
	}
	if ( !LoadGameInit() )
	{
		m_log.Info("Error", "������Ϸ����ʧ��");
		mdk::mdk_assert(false);
		exit(EXIT_FAILURE);
	}
	// ��ʼ��Redis��Ⱥ
	if ( !m_cache.InitCluster(m_cfg, 1))
	{
		m_log.Info("Error", "Redis��Ⱥ��ʼ��ʧ�ܣ�");
		mdk::mdk_assert(false);
		exit(EXIT_FAILURE);
	}

	// ���������
	srand(unsigned(time(NULL)));

	InitId("DBCenter.cfg");
	// �����˿ڣ��ȴ�����
	m_log.Info("Run", "�����˿�:%d", curSvrPORT);
	Listen(curSvrPORT);
}

Worker::~Worker(void)
{
	// body
}

bool Worker::InitId( const std::string &fileName )
{
	char exePath[1024];
	mdk::GetExeDir( exePath, 1024 );//ȡ�ÿ�ִ�г���λ��
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
	//id������
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

void Worker::OnConnect(mdk::STNetHost &host)
{
	srand(time(NULL));
}

void Worker::OnCloseConnect(mdk::STNetHost &host)
{
}

void Worker::OnMsg(mdk::STNetHost& host)
{
	msg::Buffer buffer;
	if (!host.Recv(buffer, buffer.HeaderSize(), false)) return;
	if (-1 == buffer.Size())
	{
		m_log.Info("Error", "�Ƿ����ĳ���! ǿ�ƶϿ�");
		host.Close();
		return;
	}
	if (! host.Recv(buffer, buffer.Size())) return;
	if ( buffer.IsResult() )
	{
		m_log.Info("Error", "δԤ�ϵĽ������! ǿ�ƶϿ�");
		host.Close();
		return;
	}

	switch (buffer.MoudleId())
	{
	case Moudle::Auth:    // ��֤ģ��
		OnAuth(host, buffer);
		break;
	case Moudle::SNS :
		OnSNS(host, buffer);
		break;
	case Moudle::Game :
		OnGame(host, buffer);
		break;
	default:
		m_log.Info("Error", "δԤ�ϵı���! ǿ�ƶϿ�");
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
			msg.m_reason = "���ĸ�ʽ�Ƿ�";
			break;
		}
		mdk::uint32 userId;
		Redis::Result ret = m_cache.GetUserId(msg.m_accountType, msg.m_account, userId);
		if ( Redis::unsvr == ret )
		{
			msg.m_code   = ResultCode::ResultCode(ErrorCode::EC_UserExist);
			msg.m_reason = "Redis�����쳣";
			break;
		}
		if (  Redis::success == ret )
		{
			msg.m_code   = ResultCode::ResultCode(ErrorCode::EC_UserExist);
			msg.m_reason = "�˺��Ѵ��ڣ�";
			break;
		}
		Cache::User user;
		if ( !CreateUser(msg, user) ) break;
		msg.m_userId = user.id;
	} 
	while (false);
	if ( msg.m_code )
	{
		m_log.Info("Error", "ע��ʧ��:%s!", msg.m_reason.c_str());
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
			msg.m_reason = "���ĸ�ʽ�Ƿ�";
			m_log.Info("Error", "���ĸ�ʽ�Ƿ�!");
			break;
		}
		Cache::User ui;
		ui.id = msg.m_objectId;
		if ( Redis::success != m_cache.GetUserInfo(ui) )
		{
			msg.m_code   = ResultCode::DBError;
			msg.m_reason = "��Redisʧ��";
			break;
		}
		std::string pwd = msg.m_pwd + ui.randKey;
		ui.pwd = gs_md5helper.HashString(pwd.c_str(), pwd.size());
		MySqlClient* pMysql = m_mySQLCluster.Node("Buddy", msg.m_objectId);
		if (!pMysql)
		{
			msg.m_code   = ResultCode::DBError;
			msg.m_reason = "�Ҳ���Buddy���ݿ���";
			break;
		}

		char sql[512]  = {0};
		sprintf(sql, "update user_info set pwd = \"%s\" where id = %d", 
			ui.pwd.c_str(), ui.id);
		if (!pMysql->ExecuteSql(sql))
		{
			msg.m_code   = ResultCode::DBError;
			msg.m_reason = "�޸��û���Ϣʧ��:";
			msg.m_reason += pMysql->GetLastError();
			break;
		}
		if ( !m_cache.SetUserInfo(ui) )
		{
			msg.m_code   = ResultCode::DBError;
			msg.m_reason = "д����ʧ��";
			break;
		}
	}
	while (false);
	if ( ResultCode::Success != msg.m_code )
	{
		m_log.Info("Error", "�޸�����ʧ�ܣ�%s", msg.m_reason.c_str());
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
			msg.m_reason = "���ĸ�ʽ�Ƿ�";
			break;
		}
		// Ȼ����ֻ��Ÿ������˺���Ϣ��
		MySqlClient *pMysql = m_mySQLCluster.Node("Auth", msg.m_phone);
		if (!pMysql)
		{
			msg.m_code   = ResultCode::DBError;
			msg.m_reason = "�Ҳ���Auth���ݿ���";
			break;
		}
		mdk::uint32 userId;
		char sql[256];
		sprintf(sql, "select * from bind_mobile where id = %d", msg.m_objectId);
		if ( !pMysql->ExecuteSql(sql) )
		{
			msg.m_code   = ResultCode::DBError;
			msg.m_reason = "��ѯAuth��ʧ��";
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
			msg.m_reason = "�����˺���Ϣʧ��";
			msg.m_reason += pMysql->GetLastError();
			break;
		}
	}
	while (false);
	if ( ResultCode::Success != msg.m_code )
	{
		m_log.Info("Error", "���ֻ�ʧ�ܣ�%s", msg.m_reason.c_str());
	}
	msg.Build(true);
	host.Send(msg, msg.Size());

	return true;
}

bool Worker::CreateUser(msg::UserRegister& userRegister, Cache::User& user)
{
	//////////////////////////////////////////////////////////////////////////
	//��������
	if (! CreateId(user.id)) 
	{
		userRegister.m_code   = ResultCode::Refuse;
		userRegister.m_reason = "Id��Դ���꣡";
		return false;
	}
	RandNickname(user.nickName, user.id);
	RandKey(user.randKey);
	// �������� -> MD5(userRegister.m_pwd + randKey)
	std::string pwd = userRegister.m_pwd + user.randKey;
	user.pwd = gs_md5helper.HashString(pwd.c_str(), pwd.size());

	//////////////////////////////////////////////////////////////////////////
	//д���ݿ�
	MySqlClient* pMysql = m_mySQLCluster.Node("Auth", userRegister.m_account);
	if ( !pMysql ) 
	{
		userRegister.m_code   = ResultCode::DBError;
		userRegister.m_reason = "�Ҳ���Auth���ݿ���";
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
		userRegister.m_reason = "�Ƿ��˺�����";
		return false;
	}
	if (! pMysql->ExecuteSql(sql))
	{
		userRegister.m_code   = ResultCode::DBError;
		userRegister.m_reason = "дAuth��ʧ�ܣ�";
		userRegister.m_reason += pMysql->GetLastError();
		return false;
	}
	pMysql = m_mySQLCluster.Node("Buddy", user.id);
	if ( !pMysql ) 
	{
		userRegister.m_code = ResultCode::DBError;
		userRegister.m_reason = "�Ҳ���Buddy���ݿ���";
		return false;
	}
	sprintf(sql, "insert into user_info (id, pwd, randKey, nickname, coin) "
				"values (%d, \"%s\", \"%s\", \"%s\", 1000000)", 
				user.id, user.pwd.c_str(), user.randKey.c_str(), user.nickName.c_str());
	if (! pMysql->ExecuteSql(sql))
	{
		userRegister.m_code   = ResultCode::DBError;
		userRegister.m_reason = "дBuddy��ʧ�ܣ�";
		userRegister.m_reason += pMysql->GetLastError();
		return false;
	}
	//////////////////////////////////////////////////////////////////////////
	//�޸�redis
	if ( !m_cache.SetUserInfo(user) )
	{
		userRegister.m_code   = ResultCode::DBError;
		userRegister.m_reason = "дRedisʧ��";
		return false;
	}
	if ( !m_cache.BindUserName(userRegister.m_accountType, userRegister.m_account, user.id) )
	{
		userRegister.m_code   = ResultCode::DBError;
		userRegister.m_reason = "дRedisʧ��";
		return false;
	}

	return true;
}

void Worker::RandNickname(std::string& nickname, int userId)
{
	// �ǳ����ɹ���userId��λ������Ӧ�ĳƺ� + �û�ID
	static const char* nicknames[]  = { // ���е������ǳ�
		"Ҷ����"/* 0 */, "ҹ��ҡ"/* 1 */, "����"/* 2 */, "��Ԫ̫������"/* 3 */, "�����޼�����"/* 4 */,
		"ŭ��а"/* 5 */, "������"/* 6 */, "��Ԫ���޽���"/* 7 */, "Ʈ����ѩ���㴵"/* 8 */, "̫���޼���Ԫ����"/* 9 */
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
			msg.m_reason = "���ĸ�ʽ�Ƿ�";
			break;
		}
		if ( !SetBuddy(msg.m_userId, msg.m_buddyId, msg) ) break;
		SetBuddy(msg.m_buddyId, msg.m_userId, msg);
	}
	while ( false );
	if ( msg.m_code )
	{
		m_log.Info("Error", "���С���ʧ��:%s!", msg.m_reason.c_str());
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
		msg.m_reason = "Redis�����쳣";
		return false;
	}
	if ( buddyIds.end() != buddyIds.find(buddyId) )
	{
		msg.m_code   = ResultCode::Refuse;
		msg.m_reason = "�Ѿ���С���";
		return true;
	}
	char sql[256];
	sprintf( sql, "insert into buddy (userId, buddyId) values(%u, %u)", 
		userId, buddyId );
	MySqlClient *pMysql = m_mySQLCluster.Node("Buddy", userId);
	if (!pMysql)
	{
		msg.m_code   = ResultCode::DBError;
		msg.m_reason = "�Ҳ���Buddy���ݿ���";
		return false;
	}
	if ( !pMysql->ExecuteSql(sql) )
	{
		msg.m_code   = ResultCode::DBError;
		msg.m_reason = "�޸����ݿ�ʧ��:";
		msg.m_reason += pMysql->GetLastError();
		return false;
	}
	if ( !m_cache.AddBuddy(userId, buddyId) )
	{
		msg.m_code   = ResultCode::DBError;
		msg.m_reason = "д����ʧ��";
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
			msg.m_reason = "���ĸ�ʽ�Ƿ�";
			break;
		}
		if ( !DelBuddy(msg.m_buddyId, msg.m_userId, msg) ) break;
		DelBuddy(msg.m_userId, msg.m_buddyId, msg);
	}
	while ( false );
	if ( msg.m_code )
	{
		m_log.Info("Error", "ɾ��С���ʧ��:%s!", msg.m_reason.c_str());
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
		msg.m_reason = "�Ҳ���Buddy���ݿ���";
		return false;
	}
	if ( !pMysql->ExecuteSql(sql) )
	{
		msg.m_code   = ResultCode::DBError;
		msg.m_reason = "�޸����ݿ�ʧ��:";
		msg.m_reason += pMysql->GetLastError();
		return false;
	}
	if ( !m_cache.DelBuddy(userId, buddyId) )
	{
		msg.m_code   = ResultCode::DBError;
		msg.m_reason = "д����ʧ��";
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
			msg.m_reason = "���ĸ�ʽ�Ƿ�";
			break;
		}
		SetUserData(msg);
	}
	while ( false );
	if ( msg.m_code )
	{
		m_log.Info("Error", "�����û���Ϣʧ��:%s!", msg.m_reason.c_str());
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
		msg.m_reason = "redis�޷�����";
		return false;
	}
	if ( Redis::nullData == ret )
	{
		msg.m_code   = ResultCode::DBError;
		msg.m_reason = "�û����ݲ�����";
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
		msg.m_reason = "�Ҳ���Buddy���ݿ���";
		return false;
	}
	if ( !pMysql->ExecuteSql(sql) )
	{
		msg.m_code   = ResultCode::DBError;
		msg.m_reason = "�޸����ݿ�ʧ��:";
		msg.m_reason += pMysql->GetLastError();
		return false;
	}
	if ( !m_cache.SetUserInfo(ui) )
	{
		msg.m_code   = ResultCode::DBError;
		msg.m_reason = "д����ʧ��";
		return false;
	}
	return true;
}

void Worker::OnGame(mdk::STNetHost &host, msg::Buffer &buffer)
{
	if (MsgId::setupVersion == buffer.Id()) OnSetupVersion(host, buffer);
	else if (MsgId::getPlayerData == buffer.Id()) OnGetPlayerData(host, buffer);
	else if (MsgId::buildHouse == buffer.Id()) OnBuildHouse(host, buffer);//��ͼ���ܣ���ǽ���췿��
	else if (MsgId::treePlant == buffer.Id()) OnTreePlant(host, buffer);//����
	else if (MsgId::syncPets == buffer.Id()) OnSyncPets(host, buffer);//ͬ������
	else if (MsgId::syncItem == buffer.Id()) OnSyncItem(host, buffer);//ͬ����Ʒ
	else if (MsgId::syncCoin == buffer.Id()) OnSyncCoin(host, buffer);//ͬ����Ʒ
}

bool Worker::LoadGameInit()
{
	char sql[1024];
	MySqlClient *pMysql = m_mySQLCluster.Node("GameInit");
	if (!pMysql)
	{
		m_log.Info("Error", "�Ҳ���GameInit���ݿ���");
		return false;
	}
	//ȡ�汾
	if ( !pMysql->ExecuteSql("select * from version") )
	{
		m_log.Info("Error", "���ʰ汾��Ϣʧ��:%s", pMysql->GetLastError());
		return false;
	}
	if ( pMysql->IsEmpty() )
	{
		m_log.Info("Error", "�汾��ϢΪ��");
		return false;
	}
	pMysql->MoveFirst();
	while ( pMysql->IsEof() )
	{
		pMysql->GetValue("version", m_gameVersion);
		pMysql->MoveNext();
	}
	//////////////////////////////////////////////////////////////////////////
	//ȡ����
	//ȡ����
	if ( !pMysql->ExecuteSql("select * from race ORDER BY id") )
	{
		m_log.Info("Error", "�������Ա�ʧ��:%s", pMysql->GetLastError());
		return false;
	}
	if ( pMysql->IsEmpty() )
	{
		m_log.Info("Error", "���Ա�Ϊ��");
		return false;
	}
	pMysql->MoveFirst();
	int id;
	std::string name;
	while ( !pMysql->IsEof() )
	{
		pMysql->GetValue("id", id);
		pMysql->GetValue("name", name);
		pMysql->MoveNext();
		m_races[id] = name;
	}
	std::string reason;
	//ȡ��Ʒ
	if ( !ReadItem(pMysql, m_items) ) return false;
	//ȡ����
	if ( !ReadTalent(pMysql, m_talents) ) return false;
	//ȡ����
	if ( !ReadSkill(pMysql, m_skills) ) return false;
	//ȡ�͵���
	if ( !ReadBuddy(pMysql, m_buddys) ) return false;
	//ȡ�͵��޷ֲ�ͼ
	if ( !ReadBuddyLBS(pMysql, m_buddyMaps) ) return false;

	return true;
}

bool Worker::ReadItem(MySqlClient *pMysql, std::vector<data::ITEM> &imtes)
{
	if ( !pMysql->ExecuteSql("select * from item_book ORDER BY id") )
	{
		m_log.Info("Error", "������Ʒ��ʧ��:%s", pMysql->GetLastError());
		return false;
	}
	if ( pMysql->IsEmpty() )
	{
		m_log.Info("Error", "��Ʒ��Ϊ��");
		return false;
	}
	pMysql->MoveFirst();
	data::ITEM info;
	int iVal;
	int i = 0;
	while ( !pMysql->IsEof() )
	{
		pMysql->GetValue("id", info.id);
		pMysql->GetValue("name", info.name);
		pMysql->GetValue("coin", info.coin);
		pMysql->GetValue("descript", info.descript);
		pMysql->MoveNext();
		imtes.push_back(info);
	}

	char sql[1024];
	data::EFFECT effect;
	for ( i = 0; i < imtes.size(); i++ )
	{
		sprintf( sql, "select item_book_effect.*, e1.id as effectId, bs1.id as stepId " 
			"from item_book_effect  "
			"left join ex_effect as e1 on(item_book_effect.effect = e1.`name`)  "
			"left join battle_step as bs1 on(item_book_effect.step = bs1.step)  "
			"where item_book_effect.item = \'%s\'", imtes[i].name.c_str() );
		if ( !pMysql->ExecuteSql(sql) )
		{
			m_log.Info("Error", "������Ʒ��Ч��ʧ��:%s", pMysql->GetLastError());
			return false;
		}
		if ( pMysql->IsEmpty() ) continue;
		pMysql->MoveFirst();
		int iVal;
		while (!pMysql->IsEof())
		{
			pMysql->GetValue("effectId", iVal);
			effect.id = iVal;
			pMysql->GetValue("stepId", effect.step);
			pMysql->GetValue("probability", effect.probability);
			pMysql->GetValue("agent", iVal);
			effect.agent = iVal;
			imtes[i].effects.push_back(effect);
			pMysql->MoveNext();
		}
	}

	return true;
}

bool Worker::ReadTalent(MySqlClient *pMysql, std::vector<data::TALENT> &talents)
{
	if ( !pMysql->ExecuteSql("select * from talent ORDER BY id") )
	{
		m_log.Info("Error", "�������Ա�ʧ��:%s", pMysql->GetLastError());
		return false;
	}
	if ( pMysql->IsEmpty() )
	{
		m_log.Info("Error", "���Ա�Ϊ��");
		return false;
	}
	pMysql->MoveFirst();
	data::TALENT info;
	int iVal;
	int i = 0;
	while ( !pMysql->IsEof() )
	{
		pMysql->GetValue("id", info.id);
		pMysql->GetValue("name", info.name);
		pMysql->GetValue("descript", info.descript);
		pMysql->MoveNext();
		talents.push_back(info);
	}

	char sql[1024];
	data::EFFECT effect;
	for ( i = 0; i < talents.size(); i++ )
	{
		sprintf( sql, "select talent_book_effect.*, e.id as effectId, bs.id as stepId "
			"from talent_book_effect " 
			"left join ex_effect as e on(talent_book_effect.effect = e.`name`)  "
			"left join battle_step as bs on(talent_book_effect.step = bs.step)  "
			"where talent_book_effect.talent = \'%s\'", talents[i].name.c_str() );
		if ( !pMysql->ExecuteSql(sql) )
		{
			m_log.Info("Error", "����������Ч��ʧ��:%s", pMysql->GetLastError());
			return false;
		}
		if ( pMysql->IsEmpty() ) 
		{
			m_log.Info("Error", "��������Ч");
			return false;
		}
		pMysql->MoveFirst();
		int iVal;
		while (!pMysql->IsEof())
		{
			pMysql->GetValue("effectId", iVal);
			effect.id = iVal;
			pMysql->GetValue("stepId", effect.step);
			pMysql->GetValue("probability", effect.probability);
			pMysql->GetValue("agent", iVal);
			effect.agent = iVal;
			talents[i].effects.push_back(effect);
			pMysql->MoveNext();
		}
	}

	return true;
}

bool Worker::ReadSkill(MySqlClient *pMysql, std::vector<data::SKILL> &skills)
{
	if ( !pMysql->ExecuteSql("select skill_book_info.*, r1.id as raceId from skill_book_info " 
		"left join race as r1 on(skill_book_info.race = r1.`name`) "
		"ORDER BY race, type, power") )
	{
		m_log.Info("Error", "���ʼ��ܱ�ʧ��:%s", pMysql->GetLastError());
		return false;
	}
	if ( pMysql->IsEmpty() )
	{
		m_log.Info("Error", "���ܱ�Ϊ��");
		return false;
	}
	pMysql->MoveFirst();
	data::SKILL info;
	int iVal;
	int i = 0;
	while ( !pMysql->IsEof() )
	{
		pMysql->GetValue("id", info.id);
		pMysql->GetValue("name", info.name);
		pMysql->GetValue("raceId", info.race);//����
		pMysql->GetValue("power", info.power);//����0~300
		pMysql->GetValue("type", info.type);//����
		pMysql->GetValue("hitRate", info.hitRate);//������30~101, 101���м�
		pMysql->GetValue("isMapSkill", iVal);//�ǵ�ͼ����
		info.isMapSkill = 0 == iVal?false:true;
		pMysql->GetValue("descript", info.descript);//���60byte
		if ( "" == info.descript ) info.descript = "---";
		pMysql->MoveNext();
		skills.push_back(info);
	}

	char sql[1024];
	data::EFFECT effect;
	for ( i = 0; i < skills.size(); i++ )
	{
		sprintf( sql, "select skill_book_effect.*, e1.id as effectId, bs1.id as stepId from skill_book_effect "
			"left join ex_effect as e1 on(skill_book_effect.effect = e1.`name`) "
			"left join battle_step as bs1 on(skill_book_effect.step = bs1.step) "
			"where skill_book_effect.skill = \'%s\'", skills[i].name.c_str() );
		if ( !pMysql->ExecuteSql(sql) )
		{
			m_log.Info("Error", "���ʼ�����Ч��ʧ��:%s", pMysql->GetLastError());
			return false;
		}
		if ( pMysql->IsEmpty() ) continue;
		pMysql->MoveFirst();
		int iVal;
		while (!pMysql->IsEof())
		{
			pMysql->GetValue("effectId", iVal);
			effect.id = iVal;
			pMysql->GetValue("stepId", effect.step);
			pMysql->GetValue("probability", effect.probability);
			pMysql->GetValue("agent", iVal);
			effect.agent = iVal;
			pMysql->MoveNext();
			skills[i].effects.push_back(effect);
		}
	}

	return true;
}

bool Worker::ReadBuddy(MySqlClient *pMysql, std::vector<data::BUDDY> &buddys)
{
	if ( !pMysql->ExecuteSql("select r1.id as raceId1, r2.id as raceId2, "
		"t1.id as talent1, t2.id as talent2, t3.id as talent3, "
		"buddy_book_info.* from buddy_book_info "
		"left join race as r1 on(buddy_book_info.race1 = r1.`name`) "
		"left join race as r2 on(buddy_book_info.race2 = r2.`name`) "
		"left join talent as t1 on(buddy_book_info.talent1 = t1.`name`) "
		"left join talent as t2 on(buddy_book_info.talent2 = t2.`name`) "
		"left join talent as t3 on(buddy_book_info.talent3 = t3.`name`) "
		"order by buddy_book_info.number") )
	{
		m_log.Info("Error", "���ʰ͵��ޱ�ʧ��:%s", pMysql->GetLastError());
		return false;
	}
	if ( pMysql->IsEmpty() )
	{
		m_log.Info("Error", "�͵��ޱ�Ϊ��");
		return false;
	}
	pMysql->MoveFirst();
	data::BUDDY info;
	while ( !pMysql->IsEof() )
	{
		pMysql->GetValue("number", info.number);//ȫ�����
		pMysql->GetValue("name", info.name);//����
		pMysql->GetValue("descript", info.descript);//����,���60byte
		if ( "" == info.descript ) info.descript = "---";
		pMysql->GetValue("raceId1", info.race1);//����1
		pMysql->GetValue("raceId2", info.race2);//����2
		pMysql->GetValue("talent1", info.talent1);//����1
		pMysql->GetValue("talent2", info.talent2);//����2
		pMysql->GetValue("talent3", info.talent3);//������
		pMysql->GetValue("itemId", info.itemId);//ר����Ʒ��0û��ר����Ʒ
		pMysql->GetValue("hitPoint", info.hitPoint);//Ѫ
		pMysql->GetValue("physicalA", info.physicalA);//��
		pMysql->GetValue("physicalD", info.physicalD);//��
		pMysql->GetValue("specialA", info.specialA);//�ع�
		pMysql->GetValue("specialD", info.specialD);//�ط�
		pMysql->GetValue("speed", info.speed);//�ٶ�
		pMysql->GetValue("rare", info.rare);	//������ 1~99
		pMysql->GetValue("tame", info.tame);	//ѱ���� 1~99
		pMysql->MoveNext();
		buddys.push_back(info);
	}
	//ȡ�͵��޼���
	int i = 0;
	char sql[1024];
	for ( ; i < buddys.size(); i++ )
	{
		sprintf( sql, "select bi1.number as number, si1.id as skillId, buddy_book_skill.isInit "
			"from buddy_book_skill "
			"LEFT JOIN skill_book_info as si1 on(si1.`name` = buddy_book_skill.skill) "
			"LEFT JOIN buddy_book_info as bi1 on(bi1.`name` = buddy_book_skill.buddy) "
			"where bi1.number = %d "
			"order by buddy_book_skill.isInit desc, si1.race, si1.type, si1.power", buddys[i].number );
		if ( !pMysql->ExecuteSql(sql) )
		{
			m_log.Info("Error", "���ʰ͵��޼���ʧ��:%s", pMysql->GetLastError());
			return false;
		}
		if ( pMysql->IsEmpty() )
		{
			m_log.Info("Error", "���ʰ͵��޼���Ϊ��");
			return false;
		}

		pMysql->MoveFirst();
		int skillId;
		int isInit;
		while ( !pMysql->IsEof() )
		{
			pMysql->GetValue("skillId", skillId);
			pMysql->GetValue("isInit", isInit);
			buddys[i].skills[skillId] = 0 == isInit?false:true;
			pMysql->MoveNext();
		}
	}
	//ȡ������Ϣ
	for ( i = 0; i < buddys.size(); i++ )
	{
		sprintf( sql, "select buddy_book_update.*, bi.number as upMumber from buddy_book_update "
			"left join buddy_book_info as bi on(bi.`name` = buddy_book_update.upBuddy) "
			"where buddy = '%s' "
			"ORDER BY upMumber", buddys[i].name.c_str() );
		if ( !pMysql->ExecuteSql(sql) )
		{
			m_log.Info("Error", "���ʽ�����Ϣʧ��:%s", pMysql->GetLastError());
			return false;
		}
		if ( pMysql->IsEmpty() ) continue;
		pMysql->MoveFirst();
		short upMember;
		while ( !pMysql->IsEof() )
		{
			pMysql->GetValue("upMumber", upMember);
			buddys[i].upBuddys.push_back(upMember);
			pMysql->MoveNext();
		}
	}

	return true;
}

bool Worker::ReadBuddyLBS(MySqlClient *pMysql, std::vector<data::BUDDY_MAP> &buddyMaps)
{
	if ( !pMysql->ExecuteSql("select buddy_map_lbs.*, city.id as cityId from buddy_map_lbs "
		"JOIN city on(buddy_map_lbs.city = city.`name`) "
		"ORDER BY cityId, buddy_map_lbs.spot, buddy_map_lbs.id") )
	{
		m_log.Info("Error", "���ʰ͵��޷ֲ�ͼʧ��:%s", pMysql->GetLastError());
		return false;
	}
	if ( pMysql->IsEmpty() )
	{
		m_log.Info("Error", "�ް͵��޷ֲ�ͼ");
		return false;
	}
	pMysql->MoveFirst();
	data::BUDDY_MAP info;
	int iVal;
	while ( !pMysql->IsEof() )
	{
		pMysql->GetValue("id", info.id);
		pMysql->GetValue("shape", info.shape);//��״��0Բ�Σ�1����
		pMysql->GetValue("x", info.x);//latitude
		pMysql->GetValue("y", info.y);//longitude
		pMysql->GetValue("radius", info.radius);
		pMysql->GetValue("right", info.right);//latitude
		pMysql->GetValue("bottom", info.bottom);//longitude
		pMysql->GetValue("cityId", info.city);//����
		pMysql->GetValue("spot", iVal);//����
		info.spot = 0 == iVal?false:true;
		pMysql->MoveNext();
		buddyMaps.push_back(info);
	}

	//ȡ�����͵���
	int i = 0;
	msg::BuddyMap msg;
	char sql[1024];
	for ( ; i < buddyMaps.size(); i++ )
	{
		sprintf( sql, "select buddy_map_pet.*, buddy_book_info.number as number from buddy_map_pet "
			"join buddy_book_info on (buddy_map_pet.buddy = buddy_book_info.`name`) "
			"where buddy_map_pet.id = %d "
			"ORDER BY buddy_map_pet.id, buddy_book_info.number", buddyMaps[i].id );
		if ( !pMysql->ExecuteSql(sql) ) 
		{
			m_log.Info("Error", "���ʵ����͵���ʧ��:%s", pMysql->GetLastError());
			return false;
		}
		if ( pMysql->IsEmpty() )
		{
			m_log.Info("Error", "�����ް͵���");
			return false;
		}
		pMysql->MoveFirst();
		short number;
		while ( !pMysql->IsEof() )
		{
			pMysql->GetValue("number", number);
			buddyMaps[i].buddys.push_back(number);
			pMysql->MoveNext();
		}
	}

	return true;
}

bool Worker::OnSetupVersion(mdk::STNetHost &host, msg::Buffer &buffer)
{
	msg::SetupVersion msg;
	memcpy(msg, buffer, buffer.Size());
	if ( !msg.Parse() )
	{
		msg.m_code   = ResultCode::FormatInvalid;
		msg.m_reason = "���ĸ�ʽ�Ƿ�";
		msg.Build(true);
		host.Send(msg, msg.Size());
		return false;
	}
	if ( m_gameVersion == msg.m_dataVersion )//�汾һ��
	{
		msg.m_code   = ResultCode::Success;
		msg.Build(true);
		host.Send(msg, msg.Size());
		return true;
	}
	msg.m_dataVersion = m_gameVersion;
	//////////////////////////////////////////////////////////////////////////
	//��������
	int i = 0;
	{
		std::map<unsigned char, std::string>::iterator it;
		msg::RaceMap msg;
		for ( it = m_races.begin(); it != m_races.end(); it++ )
		{
			msg.m_races[it->first] = it->second;
		}
		msg.Build();
		host.Send(msg, msg.Size());
	}
	{//����
		msg::TalentBook msg;
		for ( i = 0; i < m_talents.size(); i++ )
		{
			msg.m_talents.push_back(m_talents[i]);
			if ( 100 == msg.m_talents.size() )
			{
				msg.Build();
				host.Send(msg, msg.Size());
				msg.m_talents.clear();
			}
		}
		if ( msg.m_talents.size() > 0 )
		{
			msg.Build();
			host.Send(msg, msg.Size());
		}
	}
	{//��Ʒ
		msg::ItemBook msg;
		for ( i = 0; i < m_items.size(); i++ )
		{
			msg.m_items.push_back(m_items[i]);
			if ( 50 == msg.m_items.size()  )
			{
				msg.Build();
				host.Send(msg, msg.Size());
				msg.m_items.clear();
			}
		}
		if ( msg.m_items.size() > 0 )
		{
			msg.Build();
			host.Send(msg, msg.Size());
		}
	}
	{//����
		msg::SkillBook msg;
		for ( i = 0; i < m_skills.size(); i++ )
		{
			msg.m_skills.push_back(m_skills[i]);
			if ( 50 == msg.m_skills.size()  )
			{
				msg.Build();
				host.Send(msg, msg.Size());
				msg.m_skills.clear();
			}
		}
		if ( msg.m_skills.size() > 0 )
		{
			msg.Build();
			host.Send(msg, msg.Size());
		}
	}
	{//�͵���
		msg::BuddyBook msg;
		for ( i = 0; i < m_buddys.size(); i++ )
		{
			msg.m_buddys.push_back(m_buddys[i]);
			if ( 20 == msg.m_buddys.size()  )
			{
				msg.Build();
				host.Send(msg, msg.Size());
				msg.m_buddys.clear();
			}
		}
		if ( msg.m_buddys.size() > 0 )
		{
			msg.Build();
			host.Send(msg, msg.Size());
		}
	}
	{//��ͼ
		msg::BuddyMap msg;
		int i = 0;
		for ( i = 0; i < m_buddyMaps.size(); i++ )
		{
			msg.m_buddyMaps.push_back(m_buddyMaps[i]);
			if ( 30 == msg.m_buddyMaps.size()  )
			{
				msg.Build();
				host.Send(msg, msg.Size());
				msg.m_buddyMaps.clear();
			}
		}
		if ( msg.m_buddyMaps.size() > 0 )
		{
			msg.Build();
			host.Send(msg, msg.Size());
		}
	}
	msg.m_code = ResultCode::Success;
	msg.Build(true);
	host.Send(msg, msg.Size());
	return true;
}

void Worker::OnGetPlayerData(mdk::STNetHost &host, msg::Buffer &buffer)
{
	msg::GetPlayerData msg;
	memcpy(msg, buffer, buffer.Size());
	if ( !msg.Parse() )
	{
		msg.m_code   = ResultCode::FormatInvalid;
		msg.m_reason = "���ĸ�ʽ�Ƿ�";
		msg.Build(true);
		host.Send(msg, msg.Size());
		return;
	}
	if ( !CreatePlayer(msg.m_objectId) )
	{
		msg.m_code   = ResultCode::DBError;
		msg.m_reason = "�����������ʧ��";
		msg.Build(true);
		host.Send(msg, msg.Size());
		return;
	}

	char sql[1024];
	MySqlClient *pMysql = m_mySQLCluster.Node("GameBuddy", msg.m_objectId);
	if (!pMysql)
	{
		msg.m_code   = ResultCode::DBError;
		msg.m_reason = "�Ҳ���GameBuddy���ݿ���";
		msg.Build(true);
		host.Send(msg, msg.Size());
		return;
	}
	std::vector<data::PET> pets;//���100ֻ
	if ( !ReadPets(pMysql, msg.m_objectId, pets, msg.m_code, msg.m_reason) )
	{
		msg.Build(true);
		host.Send(msg, msg.Size());
		return;
	}
	std::vector<data::PLAYER_ITEM> items;//���200��
	if ( !ReadPlayerItems(pMysql, msg.m_objectId, items, msg.m_code, msg.m_reason) )
	{
		msg.Build(true);
		host.Send(msg, msg.Size());
		return;
	}
	{
		msg::Pets msg;
		int i = 0;
		for ( i = 0; i < pets.size(); i++ )
		{
			msg.m_pets.push_back(pets[i]);
			if ( 100 == msg.m_pets.size() )
			{
				msg.Build();
				host.Send(msg, msg.Size());
				msg.m_pets.clear();
			}
		}
		if ( msg.m_pets.size() > 0 )
		{
			msg.Build();
			host.Send(msg, msg.Size());
		}
	}
	{
		msg::PlayerItems msg;
		int i = 0;
		for ( i = 0; i < items.size(); i++ )
		{
			msg.m_items.push_back(items[i]);
			if ( 200 == msg.m_items.size() )
			{
				msg.Build();
				host.Send(msg, msg.Size());
				msg.m_items.clear();
			}
		}
		if ( msg.m_items.size() > 0 )
		{
			msg.Build();
			host.Send(msg, msg.Size());
		}
	}

	msg.m_code = ResultCode::Success;
	msg.Build(true);
	host.Send(msg, msg.Size());

	return;
}

int GetNature(const std::string &add, const std::string &dec)
{
	if ("-" == add && "-" == dec ) return 0;

	if ("WG" == add && "WF" == dec ) return 1;
	if ("WG" == add && "TG" == dec ) return 2;
	if ("WG" == add && "TF" == dec ) return 3;
	if ("WG" == add && "SD" == dec ) return 4;

	if ("WF" == add && "WG" == dec ) return 5;
	if ("WF" == add && "TG" == dec ) return 6;
	if ("WF" == add && "TF" == dec ) return 7;
	if ("WF" == add && "SD" == dec ) return 8;

	if ("TG" == add && "WG" == dec ) return 9;
	if ("TG" == add && "WF" == dec ) return 10;
	if ("TG" == add && "TF" == dec ) return 11;
	if ("TG" == add && "SD" == dec ) return 12;
										    
	if ("TF" == add && "WG" == dec ) return 13;
	if ("TF" == add && "WF" == dec ) return 14;
	if ("TF" == add && "TG" == dec ) return 15;
	if ("TF" == add && "SD" == dec ) return 16;
										    
	if ("SD" == add && "WG" == dec ) return 17;
	if ("SD" == add && "WF" == dec ) return 18;
	if ("SD" == add && "TG" == dec ) return 19;
	if ("SD" == add && "TF" == dec ) return 20;
}

bool Worker::CreatePlayer(unsigned int userId)
{
	MySqlClient *pMysql = m_mySQLCluster.Node("GameBuddy", userId);
	if ( !pMysql ) return false;

	char sql[1024];
	sprintf( sql, "select * from pet where userId = %d ", userId );
	if ( !pMysql->ExecuteSql(sql) ) return false;
	if ( !pMysql->IsEmpty() ) return true;

	sprintf( sql, "insert into player_item (userId, itemId, count) values(%d, 4, 1000) ", userId );
	if ( !pMysql->ExecuteSql(sql) ) return false;
	sprintf( sql, "insert into player_item (userId, itemId, count) values(%d, 8, 1) ", userId );
	if ( !pMysql->ExecuteSql(sql) ) return false;
	int petId = 0;
	data::BUDDY *pBuddy = NULL;
	//��ţ
	petId++;
	pBuddy = Buddy("��ţ");
	if ( !AddPet( pBuddy->number, userId, petId, pBuddy->talent1, 0, 25, 25, 25, 25, 25, 25 ) ) return false;

	//̰�Թ�
	petId++;
	pBuddy = Buddy("̰�Թ�");
	if ( !AddPet( pBuddy->number, userId, petId, pBuddy->talent1, 0, 25, 25, 25, 25, 25, 25 ) ) return false;
	//����
	petId++;
	pBuddy = Buddy("����");
	if ( !AddPet( pBuddy->number, userId, petId, pBuddy->talent1, 0, 25, 25, 25, 25, 25, 25 ) ) return false;
	//��ȸ
	petId++;
	pBuddy = Buddy("��ȸ");
	if ( !AddPet( pBuddy->number, userId, petId, pBuddy->talent1, 0, 25, 25, 25, 25, 25, 25 ) ) return false;
	//����ħż
	petId++;
	pBuddy = Buddy("����ħż");
	if ( !AddPet( pBuddy->number, userId, petId, pBuddy->talent1, 0, 25, 25, 25, 25, 25, 25 ) ) return false;
	//ҹħ��
	petId++;
	pBuddy = Buddy("ҹħ��");
	if ( !AddPet( pBuddy->number, userId, petId, pBuddy->talent1, 0, 25, 25, 25, 25, 25, 25 ) ) return false;

	return true;
}

data::BUDDY* Worker::Buddy(short number)
{
	int i = 0;
	for ( i = 0; i < m_buddys.size(); i++ )
	{
		if ( number == m_buddys[i].number ) return &m_buddys[i];
	}

	return NULL;
}

data::BUDDY* Worker::Buddy(const std::string name)
{
	int i = 0;
	for ( i = 0; i < m_buddys.size(); i++ )
	{
		if ( name == m_buddys[i].name ) return &m_buddys[i];
	}
	
	return NULL;
}

data::ITEM* Worker::Item( int itemId )
{
	int i = 0;
	for ( i = 0; i < m_items.size(); i++ )
	{
		if ( itemId == m_items[i].id ) return &m_items[i];
	}

	return NULL;
}

bool Worker::ReadPets(MySqlClient *pMysql, unsigned int userId, std::vector<data::PET> &pets, ResultCode::ResultCode &result, std::string &reason)
{
	char sql[1024];
	sprintf( sql, "select * from pet where userId = %d ", userId );
	if ( !pMysql->ExecuteSql(sql) || pMysql->IsEmpty() ) 
	{
		result = ResultCode::DBError;
		reason = "���ʳ�������ʧ��";
		return false;
	}
	data::PET pet;
	while ( !pMysql->IsEof() )
	{
		pMysql->GetValue("petId", pet.id);
		pMysql->GetValue("number", pet.number);
		pMysql->GetValue("talent", pet.talent);
		pMysql->GetValue("skill1", pet.skill1);
		pMysql->GetValue("skill2", pet.skill2);
		pMysql->GetValue("skill3", pet.skill3);
		pMysql->GetValue("skill4", pet.skill4);
		pMysql->GetValue("nature", pet.nature);
		pMysql->GetValue("HPMuscle", pet.HPHealthy);
		pMysql->GetValue("WGMuscle", pet.WGHealthy);
		pMysql->GetValue("WFMuscle", pet.WFHealthy);
		pMysql->GetValue("TGMuscle", pet.TGHealthy);
		pMysql->GetValue("TFMuscle", pet.TFHealthy);
		pMysql->GetValue("SDMuscle", pet.SDHealthy);
		pet.HPMuscle = pet.HPHealthy;
		pet.WGMuscle = pet.WGHealthy;
		pet.WFMuscle = pet.WFHealthy;
		pet.TGMuscle = pet.TGHealthy;
		pet.TFMuscle = pet.TFHealthy;
		pet.SDMuscle = pet.SDHealthy;
		pMysql->GetValue("HPHealthy", pet.HPHealthy);
		pMysql->GetValue("WGHealthy", pet.WGHealthy);
		pMysql->GetValue("WFHealthy", pet.WFHealthy);
		pMysql->GetValue("TGHealthy", pet.TGHealthy);
		pMysql->GetValue("TFHealthy", pet.TFHealthy);
		pMysql->GetValue("SDHealthy", pet.SDHealthy);
		pMysql->GetValue("itemId", pet.itemId);
		pMysql->MoveNext();
		pets.push_back(pet);
	}

	int i = 0;
	for ( i = 0; i < pets.size(); i++ )
	{
		sprintf( sql, "select * from pet_skill where userId = %d and petId = %d", userId, pets[i].id );
		if ( !pMysql->ExecuteSql(sql) ) 
		{
			result = ResultCode::DBError;
			reason = "���ʳ�������ʧ��";
			return false;
		}
		if ( pMysql->IsEmpty() ) continue;
		char race;
		pMysql->MoveFirst();
		while ( !pMysql->IsEof() )
		{
			pMysql->GetValue("race", race);
			pMysql->MoveNext();
			pets[i].race.push_back(race);
		}
	}

	return true;
}

bool Worker::ReadPlayerItems(MySqlClient *pMysql, unsigned int userId, std::vector<data::PLAYER_ITEM> &items, ResultCode::ResultCode &result, std::string &reason)
{
	char sql[1024];
	sprintf( sql, "select * from player_item where userId = %d ", userId );
	if ( !pMysql->ExecuteSql(sql) ) 
	{
		result = ResultCode::DBError;
		reason = "������Ʒ����ʧ��";
		return false;
	}
	if ( pMysql->IsEmpty() ) return true;
	data::PLAYER_ITEM item;
	while ( !pMysql->IsEof() )
	{
		pMysql->GetValue("itemId", item.id);
		pMysql->GetValue("count", item.count);
		pMysql->MoveNext();
		items.push_back(item);
	}

	return true;
}

void Worker::OnBuildHouse(mdk::STNetHost &host, msg::Buffer &buffer)
{
	msg::BuildHouse msg;
	memcpy(msg, buffer, buffer.Size());
	if ( !msg.Parse() )
	{
		msg.m_code   = ResultCode::FormatInvalid;
		msg.m_reason = "���ĸ�ʽ�Ƿ�";
		msg.Build(true);
		host.Send(msg, msg.Size());
		return;
	}
	msg.m_buildId = AddHouse(msg.m_objectId, msg.m_name, msg.m_address, msg.m_longitude, msg.m_latitude, msg.m_radius, msg.m_coin );
	if ( 0 == msg.m_buildId )
	{
		msg.m_code   = ResultCode::DBError;
		msg.m_reason = "������������ʧ��";
		msg.Build(true);
		host.Send(msg, msg.Size());
		return;
	}
	msg.m_code   = ResultCode::Success;
	msg.Build(true);
	host.Send(msg, msg.Size());
	return;
}

void Worker::OnTreePlant(mdk::STNetHost &host, msg::Buffer &buffer)
{
	msg::TreePlant msg;
	memcpy(msg, buffer, buffer.Size());
	if ( !msg.Parse() )
	{
		msg.m_code   = ResultCode::FormatInvalid;
		msg.m_reason = "���ĸ�ʽ�Ƿ�";
		msg.Build(true);
		host.Send(msg, msg.Size());
		return;
	}
	int coin = 0;
	if ( !UseItem(msg.m_objectId, 8, 1) )
	{
		msg.m_code   = ResultCode::Refuse;
		msg.m_reason = "��Ʒ����";
		msg.Build(true);
		host.Send(msg, msg.Size());
		return;
	}
	msg.m_treeId = AddTree(msg.m_objectId, msg.m_houseId);
	if ( 0 == msg.m_treeId)
	{
		msg.m_code   = ResultCode::DBError;
		msg.m_reason = "�ֹ���ʧ��";
		msg.Build(true);
		host.Send(msg, msg.Size());
		return;
	}

	msg.m_code = ResultCode::Success;
	msg.Build(true);
	host.Send(msg, msg.Size());
	return;
}

void Worker::OnSyncPets(mdk::STNetHost &host, msg::Buffer &buffer)
{
	msg::SyncPets msg;
	memcpy(msg, buffer, buffer.Size());
	if ( !msg.Parse() )
	{
		msg.m_code = ResultCode::FormatInvalid;
		msg.m_reason = "���ĸ�ʽ�Ƿ�";
		msg.Build(true);
		host.Send(msg, msg.Size());
		return;
	}
	int i = 0;
	data::BUDDY *pBuddy;
	for ( i = 0; i < msg.m_pets.size(); i++ )
	{
		pBuddy = Buddy(msg.m_pets[i].number);
		msg.m_pets[i].sync = SyncPet(msg.m_objectId, msg.m_pets[i].id, 
			msg.m_pets[i].number, msg.m_pets[i].talent, msg.m_pets[i].nature, 
			msg.m_pets[i].HPHealthy, msg.m_pets[i].WGHealthy, msg.m_pets[i].WFHealthy, 
			msg.m_pets[i].TGHealthy, msg.m_pets[i].TFHealthy, msg.m_pets[i].SDHealthy, 
			msg.m_pets[i].HPMuscle, msg.m_pets[i].WGMuscle, msg.m_pets[i].WFMuscle, 
			msg.m_pets[i].TGMuscle, msg.m_pets[i].TFMuscle, msg.m_pets[i].SDMuscle, 
			msg.m_pets[i].race);
	}
	msg.Build(true);
	host.Send(msg, msg.Size());

	return;
}

void Worker::OnSyncItem(mdk::STNetHost &host, msg::Buffer &buffer)
{
	msg::SyncItem msg;
	memcpy(msg, buffer, buffer.Size());
	if ( !msg.Parse() )
	{
		msg.m_code   = ResultCode::FormatInvalid;
		msg.m_reason = "���ĸ�ʽ�Ƿ�";
		msg.Build(true);
		host.Send(msg, msg.Size());
		return;
	}
	int i = 0;
	int coin = 0;
	for ( i = 0; i < msg.m_items.size(); i++ )
	{
		msg.m_items[i].m_successed = false;//Ĭ��ͬ��ʧ��
		if ( !SyncItem(msg.m_objectId, msg.m_items[i].m_itemId, msg.m_items[i].m_count, coin) ) continue;
		msg.m_coin += coin;
		msg.m_items[i].m_successed = true;
	}

	msg.Build(true);
	host.Send(msg, msg.Size());

	return;
}

void Worker::OnSyncCoin(mdk::STNetHost &host, msg::Buffer &buffer)
{
	msg::SyncCoin msg;
	memcpy(msg, buffer, buffer.Size());
	if ( !msg.Parse() )
	{
		msg.m_code   = ResultCode::FormatInvalid;
		msg.m_reason = "���ĸ�ʽ�Ƿ�";
		msg.Build(true);
		host.Send(msg, msg.Size());
		return;
	}
	if ( !SyncCoin(msg.m_objectId, msg.m_count) ) 
	{
		msg.m_code   = ResultCode::DBError;
		msg.m_reason = "����ʧ��";
	}
	msg.Build(true);
	host.Send(msg, msg.Size());

	return;
}

int Worker::AddHouse(unsigned int owner, const std::string &name, const std::string &address,
	const std::string &longitude, const std::string &latitude,
	int	radius, int coin )
{
	MySqlClient *pMysql = m_mySQLCluster.Node("GameBuddy", owner);
	if ( !pMysql ) return 0;

	char sql[1024];
	int houseId = 0;
	sprintf(sql, "select houseId from house where owner = %d order by houseId desc", owner);
	if (!pMysql->ExecuteSql(sql))
	{
		m_log.Info("Error", "��ѯ����id", pMysql->GetLastError());
		return 0;
	}
	if ( !pMysql->IsEmpty() ) 
	{
		pMysql->MoveFirst();
		pMysql->GetValue("houseId", houseId);
	}
	houseId++;

	sprintf(sql, "insert into house (owner, houseId, name, longitude, latitude, address, radius, coin ) "
		"values(%u, %d, \'%s\', %s, %s, \'%s\', %d, %d)",
		owner, houseId, name.c_str(), longitude.c_str(), latitude.c_str(), address.c_str(), radius, coin);
	if (!pMysql->ExecuteSql(sql))
	{
		m_log.Info("Error", "��������ʧ��:%s", pMysql->GetLastError());
		return 0;
	}

	return houseId;
}

bool Worker::UseItem(unsigned int userId, int itemId, int count)
{
	MySqlClient *pMysql = m_mySQLCluster.Node("GameBuddy", userId);
	if ( !pMysql ) return false;

	char sql[1024];
	sprintf( sql, "select * from player_item where userId = %u and itemId = %d ", 
		userId, itemId );
	if ( !pMysql->ExecuteSql(sql) ) 
	{
		m_log.Info("Error", "��ѯ��Ʒ����ʧ��:%s", sql);
		return false;
	}
	if ( pMysql->IsEmpty() ) 
	{
		m_log.Info("Error", "�û�%uû����Ʒ%d", userId, itemId);
		return false;
	}
	int curCount = 0;
	pMysql->GetValue("count", curCount);
	if ( curCount > count )
	{
		sprintf( sql, "update player_item set count += %d where userId = %u and itemId = %d ", 
			count, userId, itemId );
	}
	else
	{
		sprintf( sql, "delete from player_item where userId = %u and itemId = %d ", 
			userId, itemId );
	}
	if ( !pMysql->ExecuteSql(sql) ) 
	{
		m_log.Info("Error", "�޸���Ʒ����ʧ��:%s", sql);
		return false;
	}

	return true;
}

int Worker::AddTree(unsigned int owner, int houseId )
{
	MySqlClient *pMysql = m_mySQLCluster.Node("GameBuddy", owner);
	if ( !pMysql ) return 0;

	char sql[1024];
	int treeId = 0;
	sprintf(sql, "select treeId from tree where owner = %d order by treeId desc", owner);
	if (!pMysql->ExecuteSql(sql))
	{
		m_log.Info("Error", "��ѯ����id", pMysql->GetLastError());
		return 0;
	}
	if ( !pMysql->IsEmpty() ) 
	{
		pMysql->MoveFirst();
		pMysql->GetValue("treeId", treeId);
	}
	treeId++;

	sprintf(sql, "insert into tree (owner, treeId, houseId, createTime ) "
		"values(%u, %d, %d, now() )",
		owner, treeId, houseId );
	if (!pMysql->ExecuteSql(sql))
	{
		m_log.Info("Error", "��������ʧ��:%s", pMysql->GetLastError());
		return 0;
	}

	return houseId;
}

bool Worker::SyncItem(unsigned int userId, int itemId, int &count, int &coin)
{
	data::ITEM *pItem = Item(itemId);
	if ( NULL == pItem ) return false;

	MySqlClient *pMysql = m_mySQLCluster.Node("GameBuddy", userId);
	if ( !pMysql ) return false;

	char sql[1024];
	sprintf( sql, "select * from player_item where userId = %u and itemId = %d ", 
		userId, itemId );
	if ( !pMysql->ExecuteSql(sql) ) return false;
	coin = 0;
	if ( pMysql->IsEmpty() ) 
	{
		if ( count > 0 )
		{
			sprintf( sql, "insert into player_item (userId, itemId, count) values(%d, %d, %d) ", 
				userId, itemId, count );
		}
		else
		{
			coin = count * pItem->coin;
			if ( !SyncCoin(userId, coin) ) return false;
			coin = count * pItem->coin;
			count = 0;
		}
	}
	else
	{
		int curCount = 0;
		pMysql->GetValue("count", curCount);
		curCount = curCount + count;
		if ( curCount > 0 )
		{
			sprintf( sql, "update player_item set count = count + %d where userId = %u and itemId = %d ", 
				count, userId, itemId );
			count = curCount;
		}
		else
		{
			sprintf( sql, "delete from player_item where userId = %u and itemId = %d ", 
				userId, itemId );
			coin = curCount * pItem->coin;
			if ( !SyncCoin(userId, coin) ) return false;
			coin = curCount * pItem->coin;
			count = 0;
		}
	}
	if ( !pMysql->ExecuteSql(sql) ) m_log.Info("Error", "�޸���Ʒ����ʧ��:%s", sql);

	return true;
}

bool Worker::SyncCoin(unsigned int userId, int &count)
{
	MySqlClient *pMysql = m_mySQLCluster.Node("Buddy", userId);
	if ( !pMysql ) return false;

	char sql[1024];
	sprintf( sql, "select * from user_info where id = %u", userId );
	if ( !pMysql->ExecuteSql(sql) || pMysql->IsEmpty() )
	{
		m_log.Info("Error", "ͬ��������ʧ��:%s", sql);
		return false;
	}

	int curCoin;
	pMysql->GetValue("coin", curCoin);
	sprintf( sql, "update user_info set coin = coin + %d where id = %u ", 
		count, userId );
	if ( !pMysql->ExecuteSql(sql) ) m_log.Info("Error", "ͬ��������ʧ��:%s", sql);
	count += curCoin;

	return true;
}

bool Worker::SyncPet( unsigned int userId, int petId, 
	int number, char talent, char nature, 
	char HPHealthy, char WGHealthy, char WFHealthy, char TGHealthy, char TFHealthy, char SDHealthy,
	unsigned char HPMuscle, unsigned char WGMuscle, unsigned char WFMuscle, unsigned char TGMuscle, unsigned char TFMuscle, unsigned char SDMuscle,
	std::vector<char> race )
{
	MySqlClient *pMysql = m_mySQLCluster.Node("GameBuddy", userId);
	if ( !pMysql ) return false;

	char sql[1024];
	sprintf( sql, "select * from pet where userId = %u and petId = %d", 
		userId, petId );
	if ( !pMysql->ExecuteSql(sql) ) 
	{
		m_log.Info("Error", "�����������ʧ�ܣ�%s", sql);
		return false;
	}
	if ( pMysql->IsEmpty() ) 
	{
		sprintf( sql, "insert into pet (userId, number, petId, talent, nature, "
			"HPHealthy, WGHealthy, WFHealthy, TGHealthy, TFHealthy, SDHealthy, "
			"HPMuscle, WGMuscle, WFMuscle, TGMuscle, TFMuscle, SDMuscle) "
			"values (%u, %d, %d, %d, %d, %d,%d,%d,%d,%d,%d, %d,%d,%d,%d,%d,%d) ", 
			userId, number, petId, talent, nature,
			HPHealthy, WGHealthy, WFHealthy, TGHealthy, TFHealthy, SDHealthy,
			HPMuscle, WGMuscle, WFMuscle, TGMuscle, TFMuscle, SDMuscle );
	}
	else
	{
		sprintf( sql, "update pet "
			"set number = %d, "
			"talent = %d, "
			"HPHealthy = %d, WGHealthy = %d, WFHealthy = %d, TGHealthy = %d, TFHealthy = %d, SDHealthy = %d, "
			"HPMuscle = %d, WGMuscle = %d, WFMuscle = %d, TGMuscle = %d, TFMuscle = %d, SDMuscle = %d "
			"where userId = %u and petId = %d", 
			number, talent, 
			HPHealthy, WGHealthy, WFHealthy, TGHealthy, TFHealthy, SDHealthy,
			HPMuscle, WGMuscle, WFMuscle, TGMuscle, TFMuscle, SDMuscle,
			userId, petId);
	}
	bool sync = true;
	if ( !pMysql->ExecuteSql(sql) ) 
	{
		m_log.Info("Error", "�����������ʧ�ܣ�%s", sql);
		sync = false;
	}
	int i = 0;
	for ( i = 0; i < race.size(); i++ )
	{
		if ( 0 >= race[i] || race[i] > 17 ) continue;
		sprintf( sql, "insert into pet_skill (userId, petId, race) values(%u, %d, %d)",
			userId, petId, race[i] );
		if ( !pMysql->ExecuteSql(sql) ) 
		{
			m_log.Info("Error", "�Զ�ħ����ʧ�ܣ�%s", sql);
			sync = false;
		}
	}

	return sync;
}

bool Worker::AddPet( int number, unsigned userId, int petId, char talent, char nature, 
	char HPHealthy, char WGHealthy, char WFHealthy, char TGHealthy, char TFHealthy, char SDHealthy)
{
	std::vector<char> race;
	return SyncPet(userId, petId, number, talent, nature, 
		HPHealthy, WGHealthy, WFHealthy, TGHealthy, TFHealthy, SDHealthy,
		0,0,0,0,0,0, race );
}

