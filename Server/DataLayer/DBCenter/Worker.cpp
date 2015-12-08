#include <cstdio>
#include <ctime>
#include <cstdlib>
#include <cstring>
#include "Worker.h"
#include "common/MD5Helper.h"
#include "mdk/mapi.h"

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
	bool ret = msg.Build(true);
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
	sprintf(sql, "insert into user_info (id, pwd, randKey, nickname) "
				"values (%d, \"%s\", \"%s\", \"%s\")", 
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
	// �ǳ����ɹ����û����� + userId��λ������Ӧ������Ա�(Ĭ��Ϊ����)�ĳƺ� + �û�ID
	static const char* nicknames[]  = { // ���е������ǳ�
		"Ҷ����"/* 0 */, "ҹ��ҡ"/* 1 */, "����"/* 2 */, "Ʈ����ѩ"/* 3 */, "���㴵"/* 4 */,
		"����˪��"/* 5 */, "ŭ��а"/* 6 */, "Ʈ����ѩ���㴵"/* 7 */, "����˪��ŭ��а"/* 8 */, "̫���޼���Ԫ����"/* 9 */
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
