#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <ctime>
#include <cmath>
#include "Worker.h"
#include "mdk/mapi.h"
#include "common/common.h"
#include "common/MD5Helper.h"
#include "Protocl/cpp/Object/Auth/UserLogout.h"
#include "Protocl/cpp/Object/Auth/ResetPassword.h"
#include "Protocl/cpp/Object/Auth/BindingPhone.h"

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
				else if (Moudle::DBEntry == itMoudle->first)
				{
					m_log.Info("run", "����DBCenter���:%s %d", itNode->ip.c_str(), itNode->port);
					DBInterface* pDBInterface = new DBInterface;
					pDBInterface->SetServerInfo(itNode->ip, itNode->port);
					m_allDB.push_back(pDBInterface);
				}
				else
				{
					continue;
				}
			}
		}
	}

	if (0 == m_allDB.size())
	{
		m_log.Info("Error", "��Ⱥȱ��ģ�飬����Cluster��");
		mdk::mdk_assert(false);
		exit(0);
	}
	m_log.Info("Run", "DB�������ڵ�%d��", m_allDB.size());

	// ���ù����߳���Ŀ
	unsigned uNumbers = mdk::GetCUPNumber( 32, 4 );
	SetWorkThreadCount(uNumbers);

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
	for (int idx = 0; idx != m_allDB.size(); ++idx)
	{
		if (m_allDB[idx] != NULL)
		{
			delete m_allDB[idx];
			m_allDB[idx] = NULL;
		}
	}
	m_allDB.clear();
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
		DBInterface* pDBInterface = DBNode(msg.m_account);
		pDBInterface->CreateUser(msg); 
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
		Cache::LoginState state;
		state.flashOnline = false;		//flash������
		state.androidOnline = false;		//android������
		state.iphoneOnline = false;		//iphone������
		if ( Redis::unsvr == m_cache.GetLoginState(userId, state)) 
		{
			msg.m_code = ResultCode::DBError;
			msg.m_reason = "Redis�����쳣";
			break;
		}
		if (ClientType::android   == msg.m_clientType) state.androidOnline = true;
		else if (ClientType::iphone   == msg.m_clientType) state.iphoneOnline =  true;
		else if (ClientType::flash == msg.m_clientType) state.flashOnline =  true;
		if (!m_cache.SetLoginState(userId, state)) 
		{
			msg.m_code = ResultCode::DBError;
			msg.m_reason = "����Redisʧ�ܣ�";
			break;
		}
		// �ɹ������ظ���Ϣ
		msg.m_userId = userId;
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
	Cache::LoginState state;
	if ( Redis::unsvr == m_cache.GetLoginState(msg.m_objectId, state) )
	{
		m_log.Info("Waring", "Logoutʧ�ܣ�redis�����쳣��");
		return true;
	}
	if ( ClientType::android == msg.m_clientType ) state.androidOnline = false;
	else if ( ClientType::iphone == msg.m_clientType ) state.iphoneOnline  = false;
	if ( !m_cache.SetLoginState(msg.m_objectId, state) )
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
		DBInterface* pDBInterface = DBNode(msg.m_objectId);
		pDBInterface->ResetUserPwd(msg);
	} while (false);
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
		DBInterface* pDBInterface = DBNode(msg.m_objectId);
		pDBInterface->BindingPhone(msg);
	} while (false);
	if ( ResultCode::Success != msg.m_code )
	{
		m_log.Info("Error", "���ֻ�ʧ�ܣ�%s", msg.m_reason.c_str());
	}

	msg.Build(true);
	host.Send(msg, msg.Size());

	return (msg.m_code ? false : true);
}

DBInterface* Worker::DBNode(const std::string& strAccount)
{
	mdk::uint64 virtualId = memtoiSmall((unsigned char*)strAccount.c_str(), strAccount.size() <= 8 ? strAccount.size() : 8);
	int nodeId            = virtualId % m_allDB.size();
	return m_allDB[nodeId];
}

DBInterface* Worker::DBNode(int userId)
{
	int nodeId = userId % m_allDB.size();
	return m_allDB[nodeId];
}
