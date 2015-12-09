#include "Worker.h"
#include "mdk/mapi.h"
#include <cstring>
#include <cstdlib>
#include "common/common.h"
#include "Protocl/cpp/Object/ConnectAuth.h"
//��֤ģ����Ϣ
#include "Protocl/cpp/Object/Auth/UserRegister.h"
#include "Protocl/cpp/Object/Auth/UserLogin.h"
#include "Protocl/cpp/Object/Auth/UserLogout.h"
#include "Protocl/cpp/Object/Auth/ResetPassword.h"
#include "Protocl/cpp/Object/Auth/BindingPhone.h"
#include "Protocl/cpp/Object/Auth/UserRelogin.h"

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
	mdk::GetExeDir( exeDir, 256 );//ȡ�ÿ�ִ�г���λ��
	char cfgFile[256];
	sprintf( cfgFile, "%s/TCPEntry.cfg", exeDir );
	m_log.Info("Run", "��ȡ���ã�%s", cfgFile);
	if ( !m_cfg.ReadConfig( cfgFile ) )
	{
		m_log.Info( "Error", "���ô���" );
		mdk::mdk_assert(false);
		exit(0);
	}
	std::string ip = m_cfg["opt"]["ip"];
	int port = m_cfg["opt"]["listen"];
	m_cluster.SetSvr(m_cfg["ClusterMgr"]["ip"], m_cfg["ClusterMgr"]["port"]);
	msg::Cluster clusterInfo;
	std::string reason;
	m_log.Info( "Run", "��Ⱥ���÷���(%s %d)", ((std::string)m_cfg["ClusterMgr"]["ip"]).c_str(), 
		(int)m_cfg["ClusterMgr"]["port"] );
	if ( SyncClient::SUCESS != m_cluster.GetCluster(Moudle::all, clusterInfo, reason) )
	{
		m_log.Info( "Error", "��ȡ��Ⱥ��Ϣʧ��:%s", reason.c_str() );
		mdk::mdk_assert(false);
		exit(0);
	}
	m_authSetCount = 0;
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
				else if ( Moudle::Auth == itMoudle->first ) m_authSetCount++;
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
	if ( 0 >= m_authSetCount )
	{
		m_log.Info( "Error", "��Ⱥȱ��ģ�飬����Cluster��" );
		mdk::mdk_assert(false);
		exit(0);
	}
	m_log.Info("Run", "��֤���%d��", m_authSetCount);
	int CPU = mdk::GetCUPNumber(32, 32);
	SetWorkThreadCount(CPU);
	m_cache.InitCluster(m_cfg, CPU);


	m_log.Info( "Run", "�����˿�:%d", port );
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
		m_log.Info("Run", "client(%s:%d):����", ip.c_str(), port);
		SaveConnect(host);

		return;
	}

	ConnectInfo *pNode = (ConnectInfo*)host.GetSvrInfo();
	if ( Moudle::Auth == pNode->type )
	{
		mdk::AutoLock lock(&m_lockAuths);
		m_auths.push_back(host);
		m_log.Info("Run", "��֤����(%d):%s:%d�������", pNode->nodeId, pNode->ip.c_str(), pNode->port);
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
			m_log.Info("Error", "client(%s:%d)�Ͽ�", ip.c_str(), port);
			DelConnect(host.ID());
			return;
		}
		if ( Moudle::Client == pData->type )
		{
			UserLogout(host);
		}
		else if ( Moudle::Notify == pData->type )
		{
			m_log.Info("Run", "Notify(%d):%s:%d���ӶϿ�", 
				pData->nodeId, pData->ip.c_str(), pData->port);
		}
		
		return;
	}

	ConnectInfo *pNode = (ConnectInfo*)host.GetSvrInfo();
	if ( Moudle::Auth == pNode->type )
	{
		mdk::AutoLock lock(&m_lockAuths);
		DelFromVector(m_auths, host);
		m_log.Info("Run", "��֤����(%d):%s:%d���ӶϿ�", pNode->nodeId, pNode->ip.c_str(), pNode->port);
	}
}

void Worker::OnMsg(mdk::NetHost &host)
{
	msg::Buffer buffer; 
	if ( !host.Recv(buffer, buffer.HeaderSize(), false) ) return;
	if ( -1 == buffer.Size() )
	{
		if ( !host.IsServer() ) host.Close();
		m_log.Info("Error","�Ƿ�����");
		return;
	}
	if ( !host.Recv(buffer, buffer.Size()) ) return;
	if ( !buffer.IsResult() ) 
	{
		FillTCPParam(host, buffer);
	}
	//��ģ��ַ���Ϣ
	if ( Moudle::Auth == buffer.MoudleId() ) //��֤ģ�����⴦��
	{
		OnAuth(host, buffer);
		return;
	}
	if ( host.IsServer() ) //ҵ����Ӧ���/������������
	{
		OnServerMsg( buffer );
		return;
	}
	//client����
	if ( buffer.IsResult() ) //��Ӧ�÷��ͻ�Ӧ����
	{
		host.Close();
		return;
	}
	if ( Moudle::all == buffer.MoudleId() ) OnAllMod(host, buffer);
	else if ( Moudle::Notify == buffer.MoudleId() ) OnNotify(host, buffer);
}

bool Worker::OnServerMsg( msg::Buffer &buffer )
{
	if ( !buffer.Parse() )
	{
		m_log.Info("Error", "�Ƿ���ʽ����");
		return false;
	}
	//��Ӧ��Ϣ
	mdk::NetHost clientHost;
	if (! GetUser(buffer.m_objectId, clientHost))
	{
		if ( !GetConnect(buffer.m_connectId, clientHost ) )
		{
			m_log.Info("Waring", "�Ҳ���Client���ӣ���Ӧ����޷�����Client");
			return true;
		}
	}
	if ( ResultCode::FormatInvalid == buffer.m_code 
		|| ResultCode::MsgInvalid == buffer.m_code )
	{
		clientHost.Close();
		m_log.Info("Waring", "�ܾ�����(%llu),ԭ��(%d):%s", 
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
		m_log.Info("Error", "����������֤����ʧ��");
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
	if ( Moudle::Notify == pData->type ) modName = "Notify";
	m_log.Info("Run", "%s(%s:%d):����", modName, pData->ip.c_str(), pData->port );
	
	DelConnect(host.ID());

	return true;
}

void Worker::FillTCPParam( mdk::NetHost &host, msg::Buffer &buffer )
{
	User *pUser = (User*)host.GetData();
	if ( NULL != pUser && Moudle::Client == pUser->type ) 
	{
		buffer.m_objectId = pUser->id;//�û�id(TCP�������д)
	}
	buffer.m_connectId = host.ID();//����Id(TCP�������д)
	std::string ip;
	int port;
	host.GetAddress(ip, port);
	sscanf(ip.c_str(), "%u.%u.%u.%u", &buffer.m_ip[0], &buffer.m_ip[1], &buffer.m_ip[2], &buffer.m_ip[3]);//client ip
	buffer.Build();
}

//Auth����
bool Worker::OnAuth(mdk::NetHost &host, msg::Buffer &buffer)
{
	if ( MsgId::userLogin == buffer.Id() ) return OnUserLogin(host, buffer);//��¼�������⴦��
	if ( host.IsServer() ) return OnServerMsg( buffer );//ҵ����Ӧ���/������������
	if ( buffer.IsResult() ) //clientֻ�ܷ�����
	{
		host.Close();
		return false;
	}

	//����Ҫ��¼�Ϳ���ִ�е�ָ��
	switch ( buffer.Id() )
	{
	case MsgId::userRegister :
		return OnUserRegister(host, buffer);
	default:
		break;
	}

	//��Ҫ��¼����ִ�е�ָ��
	User *pUser = (User*)host.GetData();
	if ( NULL == pUser || Moudle::Client != pUser->type ) 
	{
		m_log.Info("Error","������Ҫ��¼���Ͽ�����");
		host.Close();
		return true;
	}
	switch ( buffer.Id() )
	{
	case MsgId::resetPassword:
	case MsgId::bindingPhone:
		{
			mdk::NetHost authHost;
			if ( !SelectAuthNode(pUser->id, authHost) ) return false;
			authHost.Send(buffer, buffer.Size());
		}
		return true;
	default:
		return true;
	}
}

bool Worker::SelectAuthNode( const std::string &accout, mdk::NetHost &host, bool randNode )
{
	mdk::uint64 virUserId = memtoiSmall((unsigned char*)accout.c_str(), accout.size() <= 8?accout.size():8);
	mdk::int32 selectNodeId = virUserId % m_authSetCount + 1;
	mdk::AutoLock lock(&m_lockAuths);
	std::vector<mdk::NetHost>::iterator it = m_auths.begin();
	ConnectInfo *pNode;
	for ( ; it != m_auths.end(); it++ )
	{
		pNode = (ConnectInfo*)it->GetSvrInfo();
		if ( selectNodeId == pNode->nodeId )
		{
			m_log.Info("Run", "Select the %dth. AuthNode", selectNodeId);
			host = *it;
			return true;
		}
	}
	if ( 0 >= m_auths.size() )
	{
		m_log.Info("Error","�޿���Auth���");
		return false;
	}
	m_log.Info("Error","%d��Auth���δ����", (virUserId % m_authSetCount) + 1);
	if ( !randNode ) return false;

	//��Ӧ������δ�ҵ���������ѯ��ʽѡ������
	static mdk::int32 pos = 0;//��ѯλ��
	if ( pos >= m_auths.size() ) pos = 0;
	host = m_auths[pos];
	pos++;

	return true;
}

bool Worker::SelectAuthNode( mdk::uint32 userId, mdk::NetHost &host )
{
	mdk::AutoLock lock(&m_lockAuths);
	int pos = userId % m_auths.size();
	host = m_auths[pos];

	return true;
}

bool Worker::OnUserRegister(mdk::NetHost &host, msg::Buffer &buffer)
{
	msg::UserRegister msg;
	memcpy(msg, buffer, buffer.Size());
	if ( !msg.Parse() )
	{
		m_log.Info("Error", "�����û�ע�ᱨ��ʧ��");
		host.Close();
		return false;
	}
	mdk::NetHost auth;
	if ( !SelectAuthNode(msg.m_account, auth) ) return false;
	auth.Send(msg, msg.Size());

	return true;
}

bool Worker::OnUserLogin(mdk::NetHost &host, msg::Buffer &buffer)
{
	if ( NULL != host.GetData() ) 
	{
		m_log.Info("Error", "�Ѿ���֤���Ӳ����Ե�¼,ǿ�ƶϿ�");
		host.Close();
		return false;
	}
	msg::UserLogin msg;
	memcpy(msg, buffer, buffer.Size());
	if ( !msg.Parse() )
	{
		m_log.Info("Error", "�����û���¼����ʧ��");
		if ( !host.IsServer() ) host.Close();
		return false;
	}

	if ( !msg.IsResult() )//��¼����ִ��ת��
	{
		mdk::NetHost auth;
		if ( !SelectAuthNode(msg.m_account, auth, false) ) return false;
		auth.Send(msg, msg.Size());

		return false;
	}
	//��Ӧ��Ϣ
	mdk::NetHost userHost;
	if ( 0 == msg.m_code )
	{
		if ( !DelConnect(msg.m_connectId, userHost) ) 
		{
			m_log.Info("Waring","�����ѶϿ�");
			return false;
		}
		SetHostRecv(userHost, true);
		User *pUser = new User;
		pUser->type = Moudle::Client;
		userHost.GetAddress(pUser->ip, pUser->port);
		pUser->id = msg.m_userId;
		pUser->clientType = msg.m_clientType;
		pUser->accountType = msg.m_accountType;
		pUser->account = msg.m_account;
		userHost.SetData(pUser);
		mdk::NetHost oldUser = AddUser(userHost);
		if ( -1 != oldUser.ID() )//֪ͨ�ɵĵ�¼���ӣ���ص�¼
		{
			msg::UserRelogin msg;
			std::string ip;
			int port;
			userHost.GetAddress(ip, port);
			char temp[64];
			sprintf(temp, "%s:%d", ip.c_str(), port);
			msg.m_position = "λ��";
			msg.m_position += temp;
			msg.Build(true);
			oldUser.Send(msg, msg.Size());
		}
	}
	else
	{
		if ( !GetConnect(msg.m_connectId, userHost) ) 
		{
			m_log.Info("Waring","�����ѶϿ�");
			return false;
		}
	}
	userHost.Send(buffer, buffer.Size());
	return true;
}

bool Worker::UserLogout(mdk::NetHost &host)
{
	ConnectInfo *pData = (ConnectInfo *)host.GetData();
	if ( NULL == pData || Moudle::Client != pData->type ) return false;
	host.Close();

	SetHostRecv(host, false);//��Ϣ����ֹͣ�����Ӷ�Ӧ�û�ת����Ϣ
	User *pUser = (User*)pData;
	m_log.Info("Run", "�û�(%d):%s�ǳ�", pUser->id, pUser->account.c_str());
	DelUser(pUser->id);

	//ת���ǳ���Ϣ��Auth
	mdk::NetHost auth;
	if ( !SelectAuthNode(pUser->account, auth, false) ) return false;
	msg::UserLogout msg;
	FillTCPParam(host, msg);
	msg.m_clientType = pUser->clientType;
	msg.Build();
	auth.Send(msg, msg.Size());
	return true;
}

//����host�Ƿ���Խ�����Ϣ����ת����Ϣ
void Worker::SetHostRecv(mdk::NetHost &host, bool enable)
{
	User *pUser = (User*)host.GetData();
	if ( NULL == pUser ) 
	{
		m_log.Info("Error", "client�����ݶ�ʧ");
		return;
	}

	if ( enable ) 
	{
		if ( !m_cache.SetUserNode(pUser->id, m_nodeId) )
		{
			m_log.Info("Error", "�����û����ڽ�����ʧ��");
			return;
		}
	}
	else  
	{
		if ( !m_cache.DelUserNode(pUser->id) )
		{
			m_log.Info("Error", "ɾ���û����ڽ�����ʧ��");
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
	if ( !buffer.Parse() )
	{
		m_log.Info("Error", "�Ƿ���ʽ֪ͨ����");
		return false;
	}
	mdk::NetHost clientHost;
	if ( !GetUser(buffer.m_objectId, clientHost) )
	{
		m_log.Info("Waring", "�Ҳ����û�(%d)���ӣ�֪ͨ�޷������û�", buffer.m_objectId);
		return true;
	}
	clientHost.Send(buffer, buffer.Size());

	return true;
}
