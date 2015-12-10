#include "Worker.h"
#include "mdk/mapi.h"
#include "Interface/DBCenter/cpp/DBCenterCluster.h"
#include "Protocl/cpp/Object/SNS/AddBuddy.h"
#include "Protocl/cpp/Object/Notify/Event.h"

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
	// ����Log���
	m_log.SetLogName("SNSSvr");
	m_log.SetPrintLog(true);

	// ��ȡ�����ļ�
	const int nBufferSize      = 256;
	char pExeDir[nBufferSize]  = {0};
	char pCfgFile[nBufferSize] = {0};

	// ȡ�ÿ�ִ�г����λ��
	mdk::GetExeDir(pExeDir, nBufferSize);
	// ȡ�������ļ���λ��
	sprintf(pCfgFile, "%s/SNSSvr.cfg", pExeDir);

	if (! m_cfg.ReadConfig(pCfgFile))
	{
		m_log.Info("Error", "SNSSvr.cfg ���ô���!");
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
				// ���ݼ�Ⱥ��Ϣ�������������Ľڵ�ID
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
				if (Moudle::Notify == itMoudle->first) ;
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

void Worker::OnConnect(mdk::NetHost &host)
{
	if ( !host.IsServer() )
	{
		std::string ip;
		int port;
		host.GetAddress(ip, port);
		m_log.Info("Run", "client(%s:%d):����", ip.c_str(), port);
		return;
	}

	ConnectInfo *pInfo = (ConnectInfo*)host.GetSvrInfo();
	if ( Moudle::Notify == pInfo->type )
	{
		m_notifyCluster.AddNode(pInfo->nodeId, host);
		m_log.Info("run", "Notify(%d):%s:%d�������", pInfo->nodeId, pInfo->ip.c_str(), pInfo->port);
	}
}

void Worker::OnCloseConnect(mdk::NetHost &host)
{
	if ( !host.IsServer() )
	{
		std::string ip;
		int port;
		host.GetAddress(ip, port);
		m_log.Info("Run", "client(%s:%d):�Ͽ�", ip.c_str(), port);
		return;
	}

	ConnectInfo *pInfo = (ConnectInfo*)host.GetSvrInfo();
	if ( Moudle::Notify == pInfo->type )
	{
		m_notifyCluster.DelNode(pInfo->nodeId);
		m_log.Info("run", "Notify(%d):%s:%d�Ͽ�", pInfo->nodeId, pInfo->ip.c_str(), pInfo->port);
	}
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
	if ( buffer.IsResult() )
	{
		m_log.Info("Error", "δԤ�ϵĻ�Ӧ,�Ͽ�����!!!" );
		host.Close();
		return;
	}

	// ����msgId������Ϣ�����������˵����Ե���Ϣ��������־
	int msgId = buffer.Id();
	if (MsgId::addBuddy == msgId) OnAddBuddy(host, buffer);
	else
	{
		std::string strIP;  // IP
		int         nPort;  // Port
		host.GetAddress(strIP, nPort);
		m_log.Info("Warning", "���Ա���!!! IP = %s, PORT = %d, msgId = %d", strIP.c_str(), nPort, msgId);
	}
}

bool Worker::OnAddBuddy(mdk::NetHost& host, msg::Buffer& buffer)
{
	msg::AddBuddy msg;
	memcpy(msg, buffer, buffer.Size());

	if ( !msg.Parse() )
	{
		msg.m_code   = ResultCode::FormatInvalid;
		msg.m_reason = "���ĸ�ʽ�Ƿ�";
		msg.Build(true);
		host.Send(msg, msg.Size());
		return true;
	}
	mdk::NetHost notifyHost;
	int nodeId = m_notifyCluster.Node(notifyHost);
	if ( 0 == nodeId )
	{
		m_log.Info("Error", "����С���ʧ�ܣ���Ϣ�����޿��÷���");
		return true;
	}
	if ( 0 == msg.m_step )
	{
		Cache::IdList buddyIds;
		if ( Redis::unsvr == m_cache.GetBuddys(msg.m_objectId, buddyIds) )
		{
			m_log.Info("Error", "����С���ʧ�ܣ�Redis�����쳣");
			return true;
		}
		int i = 0;
		for ( i = 0; i < buddyIds.m_ids.size() ; i++ )
		{
			if ( msg.m_buddyId == buddyIds.m_ids[i] )
			{
				msg.m_code   = ResultCode::Refuse;
				msg.m_reason = "�Ѿ���С���";
				msg.Build(true);
				host.Send(msg, msg.Size());
				return true;
			}
		}
	}

	Cache::User ui;
	ui.id = msg.m_objectId;
	if ( Redis::success != m_cache.GetUserInfo(ui) )
	{
		m_log.Info("Error", "����С���ʧ�ܣ����û���Ϣʧ��");
		return true;
	}
	msg.m_userId = ui.id;
	msg.m_nickName = ui.nickName;
	msg.Build();
	msg::Event notify;
	notify.m_objectId = msg.m_buddyId;
	notify.m_senderId = msg.m_userId;
	notify.m_sender = msg.m_nickName;
	notify.m_holdTime = time(NULL) + 86400 * 31;
	if ( 1 == msg.m_step )//�Է���Ӧ
	{
		if ( msg.m_accepted )//�Է�����
		{
			DBCenter *pDBCenter = ((DBCenterCluster*)SafeObject())->Node(msg.m_objectId);
			pDBCenter->AddBuddy(msg);
		}
		else //�Է��ܾ�
		{
			msg.m_code = ResultCode::Refuse;
			msg.m_reason = "�Է��ܾ�";
			msg.Build(true);
		}
	}
	memcpy(notify.m_msg, msg, msg.Size());
	notify.Build();
	notifyHost.Send(notify, notify.Size());

	return true;
}