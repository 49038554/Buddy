#include "Worker.h"
#include "mdk/mapi.h"
#include "Interface/DBCenter/cpp/DBCenterCluster.h"
#include "Protocl/cpp/Object/Notify/Event.h"

#include "Protocl/cpp/Object/SNS/AddBuddy.h"
#include "Protocl/cpp/Object/SNS/DelBuddy.h"
#include "Protocl/cpp/Object/SNS/GetBuddys.h"
#include "Protocl/cpp/Object/SNS/Buddys.h"
#include "Protocl/cpp/Object/SNS/Chat.h"
#include "Protocl/cpp/Object/SNS/SetUserData.h"
#include "Protocl/cpp/Object/SNS/GetUserData.h"
#include "Protocl/cpp/Object/SNS/UserData.h"

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

	int notifyCount = 0;
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
				if (Moudle::Notify == itMoudle->first) notifyCount++;
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
	m_notifyCluster.SetNodeCount(notifyCount);
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

	// ����msgId������Ϣ���������˵����Ե���Ϣ��������־
	int msgId = buffer.Id();
	if (MsgId::addBuddy == msgId) OnAddBuddy(host, buffer);
	else if (MsgId::delBuddy == msgId) OnDelBuddy(host, buffer);
	else if (MsgId::getBuddys == msgId) OnGetBuddys(host, buffer);
	else if (MsgId::chat == msgId) OnChat(host, buffer);
	else if (MsgId::setUserData == msgId) OnSetUserData(host, buffer);
	else if (MsgId::getUserData == msgId) OnGetUserData(host, buffer);
	else
	{
		std::string strIP;  // IP
		int         nPort;  // Port
		host.GetAddress(strIP, nPort);
		m_log.Info("Warning", "���Ա���!!! IP = %s, PORT = %d, msgId = %d", strIP.c_str(), nPort, msgId);
	}
}

bool Worker::SendNotify(mdk::uint32 recverId, unsigned char recvType, msg::Buffer *pMsg, time_t holdTime, mdk::uint32 sender, const std::string &senderName)
{
	mdk::NetHost notifyHost;
	int nodeId = m_notifyCluster.Node(recverId, notifyHost);
	if ( 0 == nodeId ) return false;

	msg::Event notify;
	notify.m_senderId = sender;
	notify.m_sender = senderName;
	notify.m_recvType = (msg::Event::RecvType)recvType;
	notify.m_recverId = recverId;
	notify.m_holdTime = holdTime;
	memcpy(notify.m_msg, *pMsg, pMsg->Size());
	notify.Build();
	notifyHost.Send(notify, notify.Size());

	return true;
}

bool Worker::OnAddBuddy(mdk::NetHost& host, msg::Buffer &buffer)
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
	if ( 0 == msg.m_step )
	{
		std::map<mdk::uint32, mdk::uint32> buddyIds;
		if ( Redis::unsvr == m_cache.GetBuddys(msg.m_objectId, buddyIds) )
		{
			msg.m_code   = ResultCode::DBError;
			msg.m_reason = "Redis�����쳣";
			msg.Build(true);
			host.Send(msg, msg.Size());
			m_log.Info("Error", "���С���ʧ�ܣ�Redis�����쳣");
			return true;
		}
		if ( buddyIds.end() != buddyIds.find(msg.m_buddyId) )
		{
			msg.m_code   = ResultCode::Refuse;
			msg.m_reason = "�Ѿ���С���";
			msg.Build(true);
			host.Send(msg, msg.Size());
			m_log.Info("Error", "�������С��飺�Ѿ���С���");
			return true;
		}
	}

	Cache::User ui;
	ui.id = msg.m_objectId;
	if ( Redis::success != m_cache.GetUserInfo(ui) )
	{
		m_log.Info("Error", "���С���ʧ�ܣ����û���Ϣʧ��");
		msg.m_code   = ResultCode::DBError;
		msg.m_reason = "���û���Ϣʧ��";
		msg.Build(true);
		host.Send(msg, msg.Size());
		return true;
	}
	msg.m_userId = ui.id;
	msg.m_nickName = ui.nickName;
	msg.Build();
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
			msg.Build();
		}
	}
	if ( !SendNotify(msg.m_buddyId, msg::Event::user, &msg, -1, msg.m_userId, msg.m_nickName) )
	{
		m_log.Info("Error", "���С���ʧ�ܣ���Ϣ�����޿��÷���");
	}

	return true;
}

bool Worker::OnDelBuddy(mdk::NetHost& host, msg::Buffer &buffer)
{
	msg::DelBuddy msg;
	memcpy(msg, buffer, buffer.Size());

	if ( !msg.Parse() )
	{
		msg.m_code   = ResultCode::FormatInvalid;
		msg.m_reason = "���ĸ�ʽ�Ƿ�";
		msg.Build(true);
		host.Send(msg, msg.Size());
		return true;
	}
	msg.m_userId = msg.m_objectId;
	msg.Build();
	DBCenter *pDBCenter = ((DBCenterCluster*)SafeObject())->Node(msg.m_userId);
	pDBCenter->DelBuddy(msg);
	if ( ResultCode::Success == msg.m_code ) 
	{
		if ( !SendNotify(msg.m_buddyId, msg::Event::user, &msg, time(NULL) + 86400 * 7) )
		{
			m_log.Info("Error", "С����Ѿ�ɾ�����޷�֪ͨ�Է�����Ϣ�����޿��÷���");
		}
	}

	msg.Build(true);
	host.Send(msg, msg.Size());

	return true;
}

bool Worker::OnGetBuddys(mdk::NetHost& host, msg::Buffer &buffer)
{
	msg::GetBuddys msg;
	memcpy(msg, buffer, buffer.Size());

	if ( !msg.Parse() )
	{
		msg.m_code   = ResultCode::FormatInvalid;
		msg.m_reason = "���ĸ�ʽ�Ƿ�";
		msg.Build(true);
		host.Send(msg, msg.Size());
		return true;
	}
	msg.m_userId = msg.m_objectId;
	std::map<mdk::uint32, mdk::uint32> buddyIds;
	if ( Redis::unsvr == m_cache.GetBuddys(msg.m_userId, buddyIds) )
	{
		msg.m_code   = ResultCode::DBError;
		msg.m_reason = "Redis�����쳣";
		msg.Build(true);
		host.Send(msg, msg.Size());
		return true;
	}

	//�����б�
	std::map<mdk::uint32, mdk::uint32>::iterator it;
	msg::Buddys buddyList;
	buddyList.m_connectId = msg.m_connectId;//����Id(TCP�������д)
	buddyList.m_objectId = msg.m_objectId;//�û�id(TCP�������д)
	memcpy(buddyList.m_ip, msg.m_ip, 4);//client ip
	buddyList.m_type = msg::Buddys::buddyList;
	buddyList.m_userId = msg.m_userId;
	msg::Buddys::BUDDY buddyItem;
	Cache::User ui;
	for ( it = buddyIds.begin(); it != buddyIds.end(); it++ )
	{
		buddyItem.id = it->second;
		ui.Clear();
		ui.id = buddyItem.id;
		buddyItem.nickName = "δ��ȡ";
		buddyItem.face = "---";
		if ( Redis::success == m_cache.GetUserInfo(ui) ) 
		{
			buddyItem.nickName = ui.nickName;
			buddyItem.face = ui.headIco;
		}
		buddyList.m_buddys.push_back(buddyItem);
		if ( 100 == buddyList.m_buddys.size() )
		{
			buddyList.Build();
			host.Send(buddyList, buddyList.Size());
			buddyList.m_buddys.clear();
		}
	}
	if ( 0 < buddyList.m_buddys.size() )
	{
		buddyList.Build();
		host.Send(buddyList, buddyList.Size());
		buddyList.m_buddys.clear();
	}

	return true;
}

bool Worker::OnChat(mdk::NetHost& host, msg::Buffer &buffer)
{
	msg::Chat msg;
	memcpy(msg, buffer, buffer.Size());

	if ( !msg.Parse() )
	{
		msg.m_code   = ResultCode::FormatInvalid;
		msg.m_reason = "���ĸ�ʽ�Ƿ�";
		msg.Build(true);
		host.Send(msg, msg.Size());
		return true;
	}
	msg.m_senderId = msg.m_objectId;
	if ( msg::Chat::buddy == msg.m_recvType )
	{
		std::map<mdk::uint32,mdk::uint32> buddyIds;
		Redis::Result ret = m_cache.GetBuddys(msg.m_senderId, buddyIds);
		if ( Redis::unsvr == ret )
		{
			m_log.Info("Error", "���ʻ���б�ʧ��");
			return false;
		}
		if ( Redis::nullData == ret || buddyIds.end() == buddyIds.find(msg.m_recverId) )
		{
			m_log.Info("Error", "���ǻ�鲻�ܷ���Ϣ");
			msg.m_code = ResultCode::Refuse;
			msg.m_reason = "����С���";
			msg.Build(true);
			host.Send(msg, msg.Size());
			return false;
		}
	}
	else if ( msg::Chat::buddys == msg.m_recvType )
	{
		std::map<mdk::uint32,mdk::uint32> buddyIds;
		Redis::Result ret = m_cache.GetBuddys(msg.m_senderId, buddyIds);
		if ( Redis::unsvr == ret )
		{
			m_log.Info("Error", "���ʻ���б�ʧ��");
			return false;
		}
		if ( Redis::nullData == ret ) return false;
	}
	else if ( msg::Chat::group == msg.m_recvType )
	{
		std::map<mdk::uint32,mdk::uint32> ids;
		Redis::Result ret = m_cache.GetGroupMember(msg.m_recverId, ids);
		if ( Redis::unsvr == ret )
		{
			m_log.Info("Error", "���ʷ����б�ʧ��");
			return false;
		}
		if ( Redis::nullData == ret || ids.end() == ids.find(msg.m_senderId) ) 
		{
			m_log.Info("Error", "�û����ڷ����У��ܾ���÷��鷢����Ϣ");
			return false;
		}
	}

	while ( false );
	Cache::User ui;
	ui.id = msg.m_senderId;
	if ( Redis::success != m_cache.GetUserInfo(ui) )
	{
		m_log.Info("Error", "����ʧ�ܣ����û���Ϣʧ��");
		msg.m_code   = ResultCode::DBError;
		msg.m_reason = "���û���Ϣʧ��";
		msg.Build(true);
		host.Send(msg, msg.Size());
		return true;
	}
	msg.m_senderName = ui.nickName;
	msg.m_senderFace = ui.headIco;
	msg.Build();
	SendNotify(msg.m_recverId, msg.m_recvType, &msg, time(NULL) + 86400 * 7, msg.m_senderId, msg.m_senderName);

	return true;
}

bool Worker::OnSetUserData(mdk::NetHost& host, msg::Buffer &buffer)
{
	msg::SetUserData msg;
	memcpy(msg, buffer, buffer.Size());

	if ( !msg.Parse() )
	{
		msg.m_code   = ResultCode::FormatInvalid;
		msg.m_reason = "���ĸ�ʽ�Ƿ�";
		msg.Build(true);
		host.Send(msg, msg.Size());
		return true;
	}
	DBCenter *pDBCenter = ((DBCenterCluster*)SafeObject())->Node(msg.m_userId);
	pDBCenter->SetUserData(msg);
	return true;
}

bool Worker::OnGetUserData(mdk::NetHost& host, msg::Buffer &buffer)
{
	msg::GetUserData msg;
	memcpy(msg, buffer, buffer.Size());

	if ( !msg.Parse() )
	{
		msg.m_code   = ResultCode::FormatInvalid;
		msg.m_reason = "���ĸ�ʽ�Ƿ�";
		msg.Build(true);
		host.Send(msg, msg.Size());
		return false;
	}
	if ( msg.m_objectId != msg.m_userId )//Ȩ�޼��
	{
		std::map<mdk::uint32,mdk::uint32> ids;
		Redis::Result ret = m_cache.GetBuddys(msg.m_objectId, ids);
		if ( Redis::unsvr == ret )
		{
			m_log.Info("Error", "��ѯ�û���Ϣʧ�ܣ��޷���ȡ�û�����б�");
			return false;
		}
		if ( Redis::nullData == ret )
		{
			msg.m_code   = ResultCode::Refuse;
			msg.m_reason = "���ܲ�ѯİ������Ϣ";
			msg.Build(true);
			host.Send(msg, msg.Size());
			return false;
		}
	}
	Cache::User ui;
	ui.id = msg.m_userId;
	Redis::Result ret = m_cache.GetUserInfo(ui);
	if ( Redis::success != ret ) 
	{
		m_log.Info("Error", "��ѯ�û���Ϣʧ�ܣ�������ʧ��");
		return false;
	}
	msg::UserData data;
	data.m_objectId = msg.m_objectId;
	data.m_userId = ui.id;
	data.m_nickName = ui.nickName;
	data.m_sex = ui.sex;
	data.m_coin = ui.coin;
	data.m_face = ui.headIco;
	data.m_bindImei = ui.bindImei;
	data.m_bindMobile = ui.bindMobile;
	data.Build();
	host.Send(data, data.Size());

	return true;
}
