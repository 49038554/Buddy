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
	// ����Log���
	m_log.SetLogName("Auth");
	m_log.SetPrintLog(true);

	// ��ȡ�����ļ�
	const int nBufferSize      = 256;
	char pExeDir[nBufferSize]  = {0};
	char pCfgFile[nBufferSize] = {0};

	// ȡ�ÿ�ִ�г����λ��
	mdk::GetExeDir(pExeDir, nBufferSize);
	// ȡ�������ļ���λ��
	sprintf(pCfgFile, "%s/Auth.cfg", pExeDir);

	if (! m_cfg.ReadConfig(pCfgFile))
	{
		m_log.Info("Error", "Auth.cfg ���ô���!");
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
				// ���ݼ�Ⱥ��Ϣ�������������Ľڵ�ID
				if (Moudle::Auth == itMoudle->first && curSvrPORT == itNode->port && curSvrIP == itNode->ip)
				{
					m_cfg["opt"]["nodeId"] = itNode->nodeId;
					m_cfg.Save();
				}
				// �������е�DB����
				if (Moudle::DBEntry == itMoudle->first) dbCenters.push_back(*itNode);
			}
		}
	}
	m_log.Info("Run", "DB�������ڵ�%d��", dbCenters.size());

	// ���ù����߳���Ŀ
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
			m_log.Info("run", "DBCenter��Ⱥ�����쳣�����id�����1��ʼ������");
			mdk::mdk_assert(false);
			exit(0);
			return;
		}
	}
	SetThreadsObjects(objList);
	// ��ʼ��Redis��Ⱥ
	if (! m_cache.InitCluster(m_cfg, uNumbers))
	{
		m_log.Info("Error", "Redis��Ⱥ��ʼ��ʧ�ܣ�");
		mdk::mdk_assert(false);
		exit(EXIT_FAILURE);
	}
	
	// ���������
	srand(unsigned(time(NULL)));

	// �����˿ڣ��ȴ�����
	m_log.Info("Run", "�����˿�:%d", curSvrPORT);
	Listen(curSvrPORT);
}

Worker::~Worker(void)
{
}

void Worker::OnCloseConnect(mdk::NetHost &host)
{
	//�������иý���ϵ�client
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
		m_log.Info("Error", "�Ƿ�����,�Ͽ�����!!!" );
		host.Close();
		return;
	}
	if (! host.Recv(buffer, buffer.Size())) return;

	// ����msgId������Ϣ���������˵����Ե���Ϣ��������־
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
		m_log.Info("Warning", "���Ա���!!! IP = %s, PORT = %d, msgId = %d", strIP.c_str(), nPort, msgId);
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
			msg.m_reason = "���ĸ�ʽ�Ƿ�";
			break;
		}
		// ע���û�
		DBCenter *pDBCenter = ((DBCenterCluster*)SafeObject())->Node(msg.m_account);
		pDBCenter->CreateUser(msg); 
	}
	while (false);
	if ( ResultCode::Success != msg.m_code )
	{
		m_log.Info("Error", "ע��ʧ�ܣ�%s", msg.m_reason.c_str());
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
			msg.m_reason = "���ĸ�ʽ�Ƿ�";
			break;
		}
		//ȡuserId���ȡ�����������ʾ�˺Ų�����
		mdk::uint32 userId = 0;
		if ( Redis::success != m_cache.GetUserId(msg.m_accountType, msg.m_account, userId) )
		{
			msg.m_code = ResultCode::ResultCode(ErrorCode::EC_AccountNotExist);
			msg.m_reason = "��ȡ�û�idʧ�ܣ�";
			break;
		}
		{//����ظ���¼
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
				msg.m_reason = "���������ط���¼";
				break;
			}
		}

		//ȡ���û���Ϣ
		Cache::User userInfo;
		userInfo.id = userId;
		if ( Redis::success != m_cache.GetUserInfo(userInfo))
		{
			msg.m_code = ResultCode::DBError;
			msg.m_reason = "��ȡ�û���Ϣʧ�ܣ�";
			break;
		}
		// �˶�����
		if (userInfo.pwd != MD5(msg.m_pwd + userInfo.randKey))
		{
			msg.m_code = ResultCode::ResultCode(ErrorCode::EC_PwdError);
			msg.m_reason = "�������";
			break;
		}

		// ���õ�½״̬
		if ( !SetLoginState(host.ID(), userId, msg.m_clientType, true) )
		{
			msg.m_code = ResultCode::DBError;
			msg.m_reason = "�����¼״̬ʧ�ܣ�";
			break;
		}

		// �ɹ������ظ���Ϣ
		msg.m_userId = userId;
		msg.m_nick = userInfo.nickName;
	}
	while (false);
	if ( ResultCode::Success != msg.m_code )
	{
		m_log.Info("Error", "��¼ʧ�ܣ�%s", msg.m_reason.c_str());
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
		m_log.Info("Error", "���ĸ�ʽ�Ƿ����Ͽ�����");
		host.Close();
		return true;
	}
	// ���õ�½״̬
	if ( !SetLoginState(host.ID(), msg.m_objectId, msg.m_clientType, false) )
	{
		m_log.Info("Waring", "Logoutʧ�ܣ�����״̬ʧ�ܣ�");
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
			msg.m_reason = "���ĸ�ʽ�Ƿ�";
			break;
		}
		// ���޸����ݿ��е�����
		DBCenter *pDBCenter = ((DBCenterCluster*)SafeObject())->Node(msg.m_objectId);
		pDBCenter->ResetUserPwd(msg);
	} 
	while (false);
	if ( ResultCode::Success != msg.m_code )
	{
		m_log.Info("Error", "�޸�����ʧ�ܣ�%s", msg.m_reason.c_str());
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
			msg.m_reason = "���ĸ�ʽ�Ƿ�";
			break;
		}
		// ���޸����ݿ��е��û���Ϣ
		DBCenter *pDBCenter = ((DBCenterCluster*)SafeObject())->Node(msg.m_objectId);
		pDBCenter->BindingPhone(msg);
	} 
	while (false);
	if ( ResultCode::Success != msg.m_code )
	{
		m_log.Info("Error", "���ֻ�ʧ�ܣ�%s", msg.m_reason.c_str());
	}

	msg.Build(true);
	host.Send(msg, msg.Size());

	return (msg.m_code ? false : true);
}
