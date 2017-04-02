#include "Worker.h"
#include "mdk/mapi.h"
#include "Interface/DBCenter/cpp/DBCenterCluster.h"
#include "Protocl/cpp/Object/Notify/Event.h"

#include "Protocl/cpp/Object/DBEntry/SetupVersion.h"
#include "Protocl/cpp/Object/DBEntry/SkillBook.h"
#include "Protocl/cpp/Object/DBEntry/ItemBook.h"
#include "Protocl/cpp/Object/DBEntry/BuddyBook.h"
#include "Protocl/cpp/Object/DBEntry/BuddyMap.h"
#include "Protocl/cpp/Object/DBEntry/RaceMap.h"

//#include "Protocl/cpp/Object/Game/CreatePlayer.h"
//#include "Protocl/cpp/Object/Game/KillBeast.h"
//#include "Protocl/cpp/Object/Game/UseItem.h"
// #include "Protocl/cpp/Object/Game/GrowUp.h"
// #include "Protocl/cpp/Object/Game/TameBeast.h"
// #include "Protocl/cpp/Object/Game/BuildHouse.h"
// #include "Protocl/cpp/Object/Game/PutPet.h"
// #include "Protocl/cpp/Object/Game/TreePlant.h"
// #include "Protocl/cpp/Object/Game/Pick.h"
// #include "Protocl/cpp/Object/Game/NearInfo.h"
// #include "Protocl/cpp/Object/Game/CreateShop.h"
// #include "Protocl/cpp/Object/Game/Buy.h"
// #include "Protocl/cpp/Object/Game/Devour.h"
// #include "Protocl/cpp/Object/Game/CatchStar.h"
// #include "Protocl/cpp/Object/Game/CityBuildInit.h"
// #include "Protocl/cpp/Object/Game/CityBuilding.h"
// #include "Protocl/cpp/Object/Game/DelHouse.h"

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
	m_log.SetLogName("GameSvr");
	m_log.SetPrintLog(true);

	// ��ȡ�����ļ�
	const int nBufferSize      = 256;
	char pExeDir[nBufferSize]  = {0};
	char pCfgFile[nBufferSize] = {0};

	// ȡ�ÿ�ִ�г����λ��
	mdk::GetExeDir(pExeDir, nBufferSize);
	// ȡ�������ļ���λ��
	sprintf(pCfgFile, "%s/GameSvr.cfg", pExeDir);

	if (! m_cfg.ReadConfig(pCfgFile))
	{
		m_log.Info("Error", "GameSvr.cfg ���ô���!");
		mdk::mdk_assert(false);
		exit(0);
	}

	std::string curSvrIP   = m_cfg["opt"]["ip"];
	int         curSvrPORT = m_cfg["opt"]["listen"];

	// �Ӽ�Ⱥ���÷���ȡ��Ⱥ������Ϣ
	m_cluster.SetService(m_cfg["ClusterMgr"]["ip"], m_cfg["ClusterMgr"]["port"]);
	m_log.Info("Run", "��Ⱥ���÷���(%s %d)", std::string(m_cfg["ClusterMgr"]["ip"]).c_str(), int(m_cfg["ClusterMgr"]["port"]));

	msg::Cluster clusterInfo;
	std::string  reason;
	if (ResultCode::success != m_cluster.GetCluster(Moudle::all, clusterInfo, reason))
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
	bool initNodeId = false;
	for (itMoudle = clusterInfo.m_cluster.begin(); itMoudle != clusterInfo.m_cluster.end(); ++itMoudle)
	{
		for (itLine = itMoudle->second.begin(); itLine != itMoudle->second.end(); ++itLine)
		{
			for (itNode = itLine->second.begin(); itNode != itLine->second.end(); ++itNode)
			{
				// ���ݼ�Ⱥ��Ϣ�������������Ľڵ�ID
				if (Moudle::Game == itMoudle->first && curSvrPORT == itNode->port && curSvrIP == itNode->ip)
				{
					m_cfg["opt"]["nodeId"] = m_nodeId = itNode->nodeId;
					m_cfg.Save();
					initNodeId = true;
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
	if ( !initNodeId )
	{
		m_log.Info("Error", "���Id���ô���");
		mdk::mdk_assert(false);
	}
	m_log.Info("Run", "DB�������ڵ�%d������Ϣ���Ľڵ�%d��", dbCenters.size(), notifyCount );
	m_notifyCluster.SetNodeCount(notifyCount);
	// ���ù����߳���Ŀ
	DBCenterCluster *pCluster = NULL;
	unsigned uNumbers = mdk::GetCUPNumber( 32, 4 );
	SetWorkThreadCount(uNumbers);
	std::vector<void*> objList;
	int i = 0;
	for ( i = 0; i < uNumbers; i++ )
	{
		pCluster = new DBCenterCluster;
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
	//��鱾���Ƿ������ݣ�û��������ݲ��ȡ
	DBCenter *pDBCenter = pCluster->Node(m_nodeId);
	if ( !pDBCenter->GetGameSetupData( m_dataVersion, m_races, m_skills, m_items, m_buddys, m_buddyMaps) )
	{
		m_log.Info("run", "��ȡ��װ����ʧ��");
		mdk::mdk_assert(false);
		exit(0);
		return;
	}

	// ��ʼ��Redis��Ⱥ
	if (! m_cache.InitCluster(m_cfg, uNumbers))
	{
		m_log.Info("Error", "Redis��Ⱥ��ʼ��ʧ�ܣ�");
		mdk::mdk_assert(false);
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
	if ( !host.Recv(buffer, buffer.HeaderSize(), false) ) return;
	if ( !buffer.ReadHeader() )
	{
		if ( !host.IsServer() ) host.Close();
		m_log.Info("Error","����ͷ����");
		return;
	}
	if ( !host.Recv(buffer, buffer.Size()) ) return;
	if ( buffer.IsResult() )
	{
		m_log.Info("Error", "δԤ�ϵĻ�Ӧ,�Ͽ�����!!!" );
		host.Close();
		return;
	}

	// ����msgId������Ϣ���������˵����Ե���Ϣ��������־
	int msgId = buffer.Id();
	switch ( msgId )
	{
		case MsgId::setupVersion://��װ�汾,��ͼ���ݣ��͵�ͼ������Ʒͼ��
			OnSetupVersion(host, buffer);
			break;
// 		case MsgId::createPlayer://�������
// 			OnCreatePlayer(host, buffer);
// 			break;
// 		case MsgId::killBeast://ɱ��Ұ��
// 			OnKillBeast(host, buffer);
// 			break;
// 		case MsgId::useItem://ʹ����Ʒ
// 			OnUseItem(host, buffer);
// 			break;
// 		case MsgId::growUp://����
// 			OnGrowUp(host, buffer);
// 			break;
// 		case MsgId::tameBeast://ѱ��Ұ��
// 			OnTameBeast(host, buffer);
// 			break;
// 		case MsgId::buildHouse://��ͼ���ܣ���ǽ���췿��
// 			OnBuildHouse(host, buffer);
// 			break;
// 		case MsgId::putPet://���ó���
// 			OnPutPet(host, buffer);
// 			break;
// 		case MsgId::treePlant://����
// 			OnTreePlant(host, buffer);
// 			break;
// 		case MsgId::pick://��ժ
// 			OnPick(host, buffer);
// 			break;
// 		case MsgId::nearInfo://�ܱ���Ϣ
// 			OnNearInfo(host, buffer);
// 			break;
// 		case MsgId::createShop://����
// 			OnCreateShop(host, buffer);
// 			break;
// 		case MsgId::buy://����
// 			OnBuy(host, buffer);
// 			break;
// 		case MsgId::devour://��ͼ���ܣ����ɣ���Ʒת��������
// 			OnDevour(host, buffer);
// 			break;
// 		case MsgId::catchStar://��ͼ���ܣ�ժ�����������ϵķ���������
// 			OnCatchStar(host, buffer);
// 			break;
// 		case MsgId::cityBuildInit://��ʼ���н���
// 			OnCityBuildInit(host, buffer);
// 			break;
// 		case MsgId::cityBuilding://Ϊ���н�����ש����
// 			OnCityBuilding(host, buffer);
// 			break;
// 		case MsgId::delHouse://��ͼ���ܣ��߸����
// 			OnDelHouse(host, buffer);
// 			break;
		default:
			break;
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

void Worker::OnSetupVersion(mdk::NetHost &host, msg::Buffer &buf)
{
	msg::SetupVersion msg;
	memcpy(msg, buf, buf.Size());
	if ( !msg.Parse() )
	{
		msg.m_code = ResultCode::msgError;
		msg.m_reason = "�Ƿ����ĸ�ʽ";
		msg.Build(true);
		host.Send(msg, msg.Size());
		return;
	}

	if ( msg.m_dataVersion == m_dataVersion )
	{
		msg.m_code = ResultCode::success;
		msg.Build(true);
		host.Send(msg, msg.Size());
		return;
	}

	//���client�汾�����ƥ�䣬�����°�����
	msg.m_dataVersion = m_dataVersion;
	{
		msg::RaceMap msg;
		msg.m_races = m_races;
		msg.Build();
		host.Send(msg, msg.Size());
	}
	{
		msg::SkillBook msg;
		int i = 0;
		int j = 0;
		while ( i < m_skills.size() )
		{
			msg.m_skills.clear();
			for ( j = 0; j < 50 && i < m_skills.size(); j++ )
			{
				msg.m_skills.push_back(m_skills[i]);
				i++;
			}
			msg.Build();
			host.Send(msg, msg.Size());
		}
	}
	{
		msg::ItemBook msg;
		int i = 0;
		int j = 0;
		while ( i < m_items.size() )
		{
			msg.m_items.clear();
			for ( j = 0; j < 50 && i < m_items.size(); j++ )
			{
				msg.m_items.push_back(m_items[i]);
				i++;
			}
			msg.Build();
			host.Send(msg, msg.Size());
		}
	}
	{
		msg::BuddyBook msg;
		int i = 0;
		int j = 0;
		while ( i < m_buddyMaps.size() )
		{
			msg.m_buddys.clear();
			for ( j = 0; j < 50 && i < m_buddys.size(); j++ )
			{
				msg.m_buddys.push_back(m_buddys[i]);
				i++;
			}
			msg.Build();
			host.Send(msg, msg.Size());
		}
	}
	{
		msg::BuddyMap msg;
		int i = 0;
		int j = 0;
		while ( i < m_buddyMaps.size() )
		{
			msg.m_buddyMaps.clear();
			for ( j = 0; j < 10 && i < m_buddyMaps.size(); j++ )
			{
				msg.m_buddyMaps.push_back(m_buddyMaps[i]);
				i++;
			}
			msg.Build();
			host.Send(msg, msg.Size());
		}
	}

	msg.m_code = ResultCode::success;
	msg.Build(true);
	host.Send(msg, msg.Size());

	return;
}

// void Worker::OnCreatePlayer(mdk::NetHost &host, msg::Buffer &buf)
// {
// 	msg::SetupVersion msg;
// 	memcpy(msg, buf, buf.Size());
// 	if ( !msg.Parse() )
// 	{
// 		msg.m_code = ResultCode::FormatInvalid;
// 		msg.m_reason = "�Ƿ����ĸ�ʽ";
// 		msg.Build(true);
// 		host.Send(msg, msg.Size());
// 		return;
// 	}
// }
// 
// void Worker::OnKillBeast(mdk::NetHost &host, msg::Buffer &buf)
// {
// 	msg::SetupVersion msg;
// 	memcpy(msg, buf, buf.Size());
// 	if ( !msg.Parse() )
// 	{
// 		msg.m_code = ResultCode::FormatInvalid;
// 		msg.m_reason = "�Ƿ����ĸ�ʽ";
// 		msg.Build(true);
// 		host.Send(msg, msg.Size());
// 		return;
// 	}
// }
// 
// void Worker::OnUseItem(mdk::NetHost &host, msg::Buffer &buf)
// {
// 	msg::SetupVersion msg;
// 	memcpy(msg, buf, buf.Size());
// 	if ( !msg.Parse() )
// 	{
// 		msg.m_code = ResultCode::FormatInvalid;
// 		msg.m_reason = "�Ƿ����ĸ�ʽ";
// 		msg.Build(true);
// 		host.Send(msg, msg.Size());
// 		return;
// 	}
// }
// 
// void Worker::OnGrowUp(mdk::NetHost &host, msg::Buffer &buf)
// {
// 	msg::SetupVersion msg;
// 	memcpy(msg, buf, buf.Size());
// 	if ( !msg.Parse() )
// 	{
// 		msg.m_code = ResultCode::FormatInvalid;
// 		msg.m_reason = "�Ƿ����ĸ�ʽ";
// 		msg.Build(true);
// 		host.Send(msg, msg.Size());
// 		return;
// 	}
// }
// 
// void Worker::OnTameBeast(mdk::NetHost &host, msg::Buffer &buf)
// {
// 	msg::SetupVersion msg;
// 	memcpy(msg, buf, buf.Size());
// 	if ( !msg.Parse() )
// 	{
// 		msg.m_code = ResultCode::FormatInvalid;
// 		msg.m_reason = "�Ƿ����ĸ�ʽ";
// 		msg.Build(true);
// 		host.Send(msg, msg.Size());
// 		return;
// 	}
// }
// 
// void Worker::OnBuildHouse(mdk::NetHost &host, msg::Buffer &buf)
// {
// 	msg::SetupVersion msg;
// 	memcpy(msg, buf, buf.Size());
// 	if ( !msg.Parse() )
// 	{
// 		msg.m_code = ResultCode::FormatInvalid;
// 		msg.m_reason = "�Ƿ����ĸ�ʽ";
// 		msg.Build(true);
// 		host.Send(msg, msg.Size());
// 		return;
// 	}
// }
// 
// void Worker::OnPutPet(mdk::NetHost &host, msg::Buffer &buf)
// {
// 	msg::SetupVersion msg;
// 	memcpy(msg, buf, buf.Size());
// 	if ( !msg.Parse() )
// 	{
// 		msg.m_code = ResultCode::FormatInvalid;
// 		msg.m_reason = "�Ƿ����ĸ�ʽ";
// 		msg.Build(true);
// 		host.Send(msg, msg.Size());
// 		return;
// 	}
// }
// 
// void Worker::OnTreePlant(mdk::NetHost &host, msg::Buffer &buf)
// {
// 	msg::SetupVersion msg;
// 	memcpy(msg, buf, buf.Size());
// 	if ( !msg.Parse() )
// 	{
// 		msg.m_code = ResultCode::FormatInvalid;
// 		msg.m_reason = "�Ƿ����ĸ�ʽ";
// 		msg.Build(true);
// 		host.Send(msg, msg.Size());
// 		return;
// 	}
// }
// 
// void Worker::OnPick(mdk::NetHost &host, msg::Buffer &buf)
// {
// 	msg::SetupVersion msg;
// 	memcpy(msg, buf, buf.Size());
// 	if ( !msg.Parse() )
// 	{
// 		msg.m_code = ResultCode::FormatInvalid;
// 		msg.m_reason = "�Ƿ����ĸ�ʽ";
// 		msg.Build(true);
// 		host.Send(msg, msg.Size());
// 		return;
// 	}
// }
// 
// void Worker::OnNearInfo(mdk::NetHost &host, msg::Buffer &buf)
// {
// 	msg::SetupVersion msg;
// 	memcpy(msg, buf, buf.Size());
// 	if ( !msg.Parse() )
// 	{
// 		msg.m_code = ResultCode::FormatInvalid;
// 		msg.m_reason = "�Ƿ����ĸ�ʽ";
// 		msg.Build(true);
// 		host.Send(msg, msg.Size());
// 		return;
// 	}
// }
// 
// void Worker::OnCreateShop(mdk::NetHost &host, msg::Buffer &buf)
// {
// 	msg::SetupVersion msg;
// 	memcpy(msg, buf, buf.Size());
// 	if ( !msg.Parse() )
// 	{
// 		msg.m_code = ResultCode::FormatInvalid;
// 		msg.m_reason = "�Ƿ����ĸ�ʽ";
// 		msg.Build(true);
// 		host.Send(msg, msg.Size());
// 		return;
// 	}
// }
// 
// void Worker::OnBuy(mdk::NetHost &host, msg::Buffer &buf)
// {
// 	msg::SetupVersion msg;
// 	memcpy(msg, buf, buf.Size());
// 	if ( !msg.Parse() )
// 	{
// 		msg.m_code = ResultCode::FormatInvalid;
// 		msg.m_reason = "�Ƿ����ĸ�ʽ";
// 		msg.Build(true);
// 		host.Send(msg, msg.Size());
// 		return;
// 	}
// }
// 
// void Worker::OnDevour(mdk::NetHost &host, msg::Buffer &buf)
// {
// 	msg::SetupVersion msg;
// 	memcpy(msg, buf, buf.Size());
// 	if ( !msg.Parse() )
// 	{
// 		msg.m_code = ResultCode::FormatInvalid;
// 		msg.m_reason = "�Ƿ����ĸ�ʽ";
// 		msg.Build(true);
// 		host.Send(msg, msg.Size());
// 		return;
// 	}
// }
// 
// void Worker::OnCatchStar(mdk::NetHost &host, msg::Buffer &buf)
// {
// 	msg::SetupVersion msg;
// 	memcpy(msg, buf, buf.Size());
// 	if ( !msg.Parse() )
// 	{
// 		msg.m_code = ResultCode::FormatInvalid;
// 		msg.m_reason = "�Ƿ����ĸ�ʽ";
// 		msg.Build(true);
// 		host.Send(msg, msg.Size());
// 		return;
// 	}
// }
// 
// void Worker::OnCityBuildInit(mdk::NetHost &host, msg::Buffer &buf)
// {
// 	msg::SetupVersion msg;
// 	memcpy(msg, buf, buf.Size());
// 	if ( !msg.Parse() )
// 	{
// 		msg.m_code = ResultCode::FormatInvalid;
// 		msg.m_reason = "�Ƿ����ĸ�ʽ";
// 		msg.Build(true);
// 		host.Send(msg, msg.Size());
// 		return;
// 	}
// }
// 
// void Worker::OnCityBuilding(mdk::NetHost &host, msg::Buffer &buf)
// {
// 	msg::SetupVersion msg;
// 	memcpy(msg, buf, buf.Size());
// 	if ( !msg.Parse() )
// 	{
// 		msg.m_code = ResultCode::FormatInvalid;
// 		msg.m_reason = "�Ƿ����ĸ�ʽ";
// 		msg.Build(true);
// 		host.Send(msg, msg.Size());
// 		return;
// 	}
// }
// 
// void Worker::OnDelHouse(mdk::NetHost &host, msg::Buffer &buf)
// {
// 	msg::SetupVersion msg;
// 	memcpy(msg, buf, buf.Size());
// 	if ( !msg.Parse() )
// 	{
// 		msg.m_code = ResultCode::FormatInvalid;
// 		msg.m_reason = "�Ƿ����ĸ�ʽ";
// 		msg.Build(true);
// 		host.Send(msg, msg.Size());
// 		return;
// 	}
// }
