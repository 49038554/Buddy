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
	m_cluster.SetService(m_cfg["ClusterMgr"]["ip"], m_cfg["ClusterMgr"]["port"]);
	msg::Cluster clusterInfo;
	std::string reason;
	m_log.Info( "Run", "��Ⱥ���÷���(%s %d)", ((std::string)m_cfg["ClusterMgr"]["ip"]).c_str(), 
		(int)m_cfg["ClusterMgr"]["port"] );
	if ( ResultCode::success != m_cluster.GetCluster(Moudle::all, clusterInfo, reason) )
	{
		m_log.Info( "Error", "��ȡ��Ⱥ��Ϣʧ��:%s", reason.c_str() );
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
	m_log.Info("Run", "��֤���%d��, sns���%d��, DBEntry���%d��", m_authSvrCount, m_snsSvrCount, m_dbSvrCount);
	if ( 0 >= m_authSvrCount || 0 >= m_snsSvrCount || 0 >= m_dbSvrCount )
	{
		m_log.Info( "Error", "��Ⱥȱ��ģ�飬����Cluster��" );
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
		m_authCluster.AddNode(pNode->nodeId, host);
		m_log.Info("Run", "��֤����(%d):%s:%d�������", pNode->nodeId, pNode->ip.c_str(), pNode->port);
	}
	if ( Moudle::SNS == pNode->type )
	{
		m_snsCluster.AddNode(pNode->nodeId, host);
		m_log.Info("Run", "SNS����(%d):%s:%d�������", pNode->nodeId, pNode->ip.c_str(), pNode->port);
	}
	if ( Moudle::DBEntry == pNode->type )
	{
		m_dbCluster.AddNode(pNode->nodeId, host);
		m_log.Info("Run", "���ݷ���(%d):%s:%d�������", pNode->nodeId, pNode->ip.c_str(), pNode->port);
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
			m_notifyCluster.DelNode(pData->nodeId);
		}
		
		return;
	}

	ConnectInfo *pNode = (ConnectInfo*)host.GetSvrInfo();
	if ( Moudle::Auth == pNode->type )
	{
		m_authCluster.DelNode(pNode->nodeId);
		m_log.Info("Run", "��֤����(%d):%s:%d���ӶϿ�", pNode->nodeId, pNode->ip.c_str(), pNode->port);
	}
	if ( Moudle::SNS == pNode->type )
	{
		m_snsCluster.DelNode(pNode->nodeId);
		m_log.Info("Run", "SNS����(%d):%s:%d���ӶϿ�", pNode->nodeId, pNode->ip.c_str(), pNode->port);
	}
	if ( Moudle::DBEntry == pNode->type )
	{
		m_dbCluster.DelNode(pNode->nodeId);
		m_log.Info("Run", "���ݷ���(%d):%s:%d���ӶϿ�", pNode->nodeId, pNode->ip.c_str(), pNode->port);
	}
	
}

void Worker::OnMsg(mdk::NetHost &host)
{
	msg::Buffer buffer; 
	if ( !host.Recv(buffer, buffer.HeaderSize(), false) ) return;
	if ( !buffer.ReadHeader() )
	{
		if ( !host.IsServer() ) host.Close();
		m_log.Info("Error","����ͷ����");
		return;
	}
	if ( !host.Recv(buffer, buffer.Size()) ) return;
	if ( Moudle::Notify == buffer.MoudleId() ) 
	{
		OnNotify(host, buffer);
		return;
	}

	if ( !buffer.IsResult() ) FillTCPParam(host, buffer);
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
	if ( !buffer.Parse() )
	{
		host.Close();
		m_log.Info("Error","�Ƿ�����ͷ��ǿ�ƶϿ�");
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
			m_log.Info("Error", "DBEntry���δ����");
			return;
		}
		helperHost.Send(buffer, buffer.Size());
		return;
	}
	if ( buffer.IsResult() ) //��Ӧ�÷��ͻ�Ӧ����
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
	//ҵ��ָ��
	User *pUser = (User*)host.GetData();
	if ( NULL == pUser || Moudle::Client != pUser->type ) //�����¼
	{
		m_log.Info("Error","������Ҫ��¼���Ͽ�����");
		host.Close();
		return;
	}
	mdk::NetHost helperHost;
	int nodeId = 0;
	if ( Moudle::SNS == buffer.MoudleId() ) nodeId = m_snsCluster.Node(helperHost);
	if ( Moudle::DBEntry == buffer.MoudleId() ) nodeId = m_dbCluster.Node(helperHost);
	if ( 0 == nodeId )
	{
		m_log.Info("Error", "������δ����");
		return;
	}
	helperHost.Send(buffer, buffer.Size());

	return;
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
	if ( ResultCode::msgError == buffer.m_code )
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
	if ( Moudle::Notify == pData->type ) 
	{
		modName = "Notify";
		m_notifyCluster.AddNode(pData->nodeId, host);
	}
	m_log.Info("Run", "%s(%d):%s:%d�ѵ�¼", modName, pData->nodeId, pData->ip.c_str(), pData->port );
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
			int nodeId = m_authCluster.Node(pUser->id, authHost);
			if ( 0 == nodeId )
			{
				m_log.Info("Error", "Auth���δ����");
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
		m_log.Info("Error", "�����û�ע�ᱨ��ʧ��");
		host.Close();
		return false;
	}
	mdk::NetHost auth;
	int nodeId = m_authCluster.Node(auth);
	if ( 0 == nodeId ) 
	{
		m_log.Info("Error", "��auth���");
		return false;
	}
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
		mdk::uint32 userId = 0;
		Redis::Result ret = m_cache.GetUserId(msg.m_accountType, msg.m_account, userId);
		if ( Redis::unsvr == ret )
		{
			msg.m_code = ResultCode::refuse;
			msg.m_reason = "Redisδ����";
			m_log.Info("Error", "��¼ʧ�ܣ�GetUserId��Redisδ����");
			msg.Build(true);
			host.Send(msg, msg.Size());
			return false;
		}
		if ( Redis::nullData == ret )
		{
			msg.m_code = ResultCode::refuse;
			msg.m_reason = "�˺Ų�����";
			m_log.Info("Error", "�˺�(%s)������", msg.m_account.c_str());
			msg.Build(true);
			host.Send(msg, msg.Size());
			return false;
		}
		mdk::NetHost auth;
		int nodeId = m_authCluster.Node(userId, auth);
		if ( 0 == nodeId )
		{
			m_log.Info("Error", "Auth���δ����");
			return false;
		}
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
		if ( "���������ط���¼" == msg.m_reason ) NotifyRelogin(msg);
		if ( !GetConnect(msg.m_connectId, userHost) ) 
		{
			m_log.Info("Waring","�����ѶϿ�");
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
		m_log.Info("Error", "��ѯ�û�idʧ�ܣ��޷�֪ͨ�ѵ�¼�û�(%u)����", userId);
		return;
	}
	mdk::int32 tcpEntryId;
	Redis::Result ret = m_cache.GetUserNode(userId, tcpEntryId);
	if ( Redis::unsvr == ret )
	{
		m_log.Info("Error", "�޷�����redis���޷����û�(%u)������", userId);
		return;
	}
	if ( Redis::nullData == ret ) 
	{
		m_log.Info("Error", "�û�(%u)������,�������˶���", userId);
		return;
	}
	mdk::NetHost userHost;
	std::string position = "δ֪";
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
		m_log.Info("Error", "��Ϣ����δ���ӣ��������û�(%u)����", userId);
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

	SetHostRecv(host, false);//��Ϣ����ֹͣ�����Ӷ�Ӧ�û�ת����Ϣ
	User *pUser = (User*)pData;
	m_log.Info("Run", "�û�(%d):%s�ǳ�", pUser->id, pUser->account.c_str());
	DelUser(pUser->id);

	//ת���ǳ���Ϣ��Auth
	mdk::NetHost auth;
	int nodeId = m_authCluster.Node(pUser->id, auth);
	if ( 0 == nodeId )
	{
		m_log.Info("Error", "Auth���δ����");
		return false;
	}
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
	if ( MsgId::event == buffer.Id() ) 
	{
		if ( !buffer.Parse() ) 
		{
			m_log.Info("Error","֪ͨ����:�Ƿ���ʽ");
			return false;
		}
		return NotifyUser(buffer);
	}

	//ת������Ϣ���ģ����Ӧ
	if ( !buffer.IsResult() ) FillTCPParam(host, buffer);

	if ( MsgId::getEvent == buffer.Id() )
	{
		if ( !buffer.Parse() ) 
		{
			m_log.Info("Error","����Ϣ����:�Ƿ���ʽ");
			return false;
		}
		if ( !buffer.IsResult() )//ת��
		{
			mdk::NetHost msgCenter;
			if ( 0 == m_notifyCluster.Node(buffer.m_objectId, msgCenter) )
			{
				m_log.Info("Error", "��Ϣ����δ����");
				return false;
			}
			msgCenter.Send(buffer, buffer.Size());
		}
		else
		{
			mdk::NetHost clientHost;
			if ( !GetUser(buffer.m_objectId, clientHost) )
			{
				m_log.Info("Waring", "�Ҳ����û�(%d)���ӣ�֪ͨ�޷������û�", buffer.m_objectId);
				return false;
			}

			std::vector<std::string> events;
			Redis::Result ret = m_cache.GetEvents(buffer.m_objectId, events);
			if ( Redis::unsvr == ret )
			{
				m_log.Info("Error", "redisδ����");
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
					m_log.Info("Error", "1����Ϣ�޷�����");
					continue;
				}
				memcpy(buffer, data.m_msg, data.m_msg.Size());
				if ( !buffer.Parse() ) 
				{
					m_log.Info("Error", "1����Ϣ�޷�����");
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
		m_log.Info("Error", "�Ƿ���ʽ֪ͨ");
		return false;
	}
	if ( msg::Event::user != msg.m_recvType ) 
	{
		m_log.Info("Error", "δԤ�ϵ�Ⱥ����Ϣ");
		return false;
	}
	mdk::NetHost clientHost;
	if ( !GetUser(msg.m_recverId, clientHost) )
	{
		m_log.Info("Waring", "�Ҳ����û�(%d)���ӣ�֪ͨ�޷������û�", msg.m_recverId);
		return false;
	}
	memcpy(buffer, msg.m_msg, msg.m_msg.Size());
	if ( !buffer.Parse() )
	{
		m_log.Info("Error", "�Ƿ���ʽ����");
		return false;
	}
	clientHost.Send(buffer, buffer.Size());
	return true;
}
