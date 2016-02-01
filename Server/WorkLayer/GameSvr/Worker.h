#ifndef __WORKER_H__
#define __WORKER_H__

#include <string>
#include <vector>
#include <map>
#include "mdk_ex/ServerEx.h"
#include "frame/netserver/NetHost.h"
#include "frame/netserver/HostData.h"
#include "mdk/Lock.h"
#include "mdk/Logger.h"
#include "mdk/ConfigFile.h"
#include "Protocl/cpp/Buffer.h"
#include "DBCenter/CacheInterface.h"
#include "Interface/ClusterMgr/cpp/ClusterMgr.h"
#include "common/HostCluster.h"
#include "Protocl/cpp/Object/Game/SkillBook.h"
#include "Protocl/cpp/Object/Game/ItemBook.h"
#include "Protocl/cpp/Object/Game/BuddyBook.h"
#include "Protocl/cpp/Object/Game/BuddyMap.h"

/**
 * Worker
 */
class Worker : public mdk::ServerEx
{
public:
	Worker(void);
	virtual ~Worker(void);

	// ���Բ��������Ϣ������¼Log
	virtual void OnConnect(mdk::NetHost &host);
	virtual void OnCloseConnect(mdk::NetHost &host);
	virtual void OnMsg(mdk::NetHost& host);

protected:
	void OnSetupVersion(mdk::NetHost &host, msg::Buffer &buf); //��װ�汾,��ͼ���ݣ��͵�ͼ������Ʒͼ��
	void OnCreatePlayer(mdk::NetHost &host, msg::Buffer &buf); //�������
	void OnKillBeast(mdk::NetHost &host, msg::Buffer &buf); //ɱ��Ұ��
	void OnUseItem(mdk::NetHost &host, msg::Buffer &buf); //ʹ����Ʒ
	void OnGrowUp(mdk::NetHost &host, msg::Buffer &buf); //����
	void OnTameBeast(mdk::NetHost &host, msg::Buffer &buf); //ѱ��Ұ��
	void OnBuildHouse(mdk::NetHost &host, msg::Buffer &buf); //��ͼ���ܣ���ǽ���췿��
	void OnPutPet(mdk::NetHost &host, msg::Buffer &buf); //���ó���
	void OnTreePlant(mdk::NetHost &host, msg::Buffer &buf); //����
	void OnPick(mdk::NetHost &host, msg::Buffer &buf); //��ժ
	void OnNearInfo(mdk::NetHost &host, msg::Buffer &buf); //�ܱ���Ϣ
	void OnCreateShop(mdk::NetHost &host, msg::Buffer &buf); //����
	void OnBuy(mdk::NetHost &host, msg::Buffer &buf); //����
	void OnDevour(mdk::NetHost &host, msg::Buffer &buf); //��ͼ���ܣ����ɣ���Ʒת��������
	void OnCatchStar(mdk::NetHost &host, msg::Buffer &buf); //��ͼ���ܣ�ժ�����������ϵķ���������
	void OnCityBuildInit(mdk::NetHost &host, msg::Buffer &buf); //��ʼ���н���
	void OnCityBuilding(mdk::NetHost &host, msg::Buffer &buf); //Ϊ���н�����ש����
	void OnDelHouse(mdk::NetHost &host, msg::Buffer &buf); //��ͼ���ܣ��߸����

private:
	friend int main(int argc, char* argv[]);
	bool SendNotify(mdk::uint32 recverId, unsigned char recvType, msg::Buffer *pMsg, time_t holdTime = -1, mdk::uint32 sender = 0, const std::string &senderName = "ϵͳ");
	//�����Ϣ
	class ConnectInfo : public mdk::HostData
	{
	public:
		mdk::int32			nodeId;//���id
		Moudle::Moudle		type;//�������
		NetLine::NetLine	lineType;//��Ӫ����·
		std::string			ip;//��ַ
		mdk::int32			port;//�˿�
	public:
		ConnectInfo(void){}
		virtual ~ConnectInfo(){}
	};

private:
	mdk::Logger					m_log;
	mdk::ConfigFile				m_cfg;
	ClusterMgr					m_cluster;
	CacheInterface				m_cache;
	HostCluster					m_notifyCluster;
	mdk::uint16					m_nodeId;

	//ҵ������
	unsigned short								m_raceVersion;
	std::map<unsigned char, std::string>		m_races;
	mdk::uint16									m_skillVersion;
	std::vector<data::SKILL>					m_skills;
	mdk::uint16									m_itemVersion;
	std::vector<data::ITEM>						m_items;
	mdk::uint16									m_buddyVersion;
	std::vector<data::BUDDY>					m_buddys;
	mdk::uint16									m_lbsVersion;
	std::vector<data::BUDDY_MAP>				m_buddyMaps;

};

#endif // __WORKER_H__