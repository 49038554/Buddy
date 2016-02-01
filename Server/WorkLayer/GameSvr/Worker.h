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

	// 忽略不处理的消息，并记录Log
	virtual void OnConnect(mdk::NetHost &host);
	virtual void OnCloseConnect(mdk::NetHost &host);
	virtual void OnMsg(mdk::NetHost& host);

protected:
	void OnSetupVersion(mdk::NetHost &host, msg::Buffer &buf); //安装版本,地图数据，巴迪图鉴，物品图鉴
	void OnCreatePlayer(mdk::NetHost &host, msg::Buffer &buf); //创建玩家
	void OnKillBeast(mdk::NetHost &host, msg::Buffer &buf); //杀死野怪
	void OnUseItem(mdk::NetHost &host, msg::Buffer &buf); //使用物品
	void OnGrowUp(mdk::NetHost &host, msg::Buffer &buf); //进化
	void OnTameBeast(mdk::NetHost &host, msg::Buffer &buf); //驯服野怪
	void OnBuildHouse(mdk::NetHost &host, msg::Buffer &buf); //地图技能：光墙，造房子
	void OnPutPet(mdk::NetHost &host, msg::Buffer &buf); //放置宠物
	void OnTreePlant(mdk::NetHost &host, msg::Buffer &buf); //种树
	void OnPick(mdk::NetHost &host, msg::Buffer &buf); //采摘
	void OnNearInfo(mdk::NetHost &host, msg::Buffer &buf); //周边信息
	void OnCreateShop(mdk::NetHost &host, msg::Buffer &buf); //开店
	void OnBuy(mdk::NetHost &host, msg::Buffer &buf); //购买
	void OnDevour(mdk::NetHost &host, msg::Buffer &buf); //地图技能：吞噬，物品转换正能量
	void OnCatchStar(mdk::NetHost &host, msg::Buffer &buf); //地图技能：摘星术，将天上的法宝打下来
	void OnCityBuildInit(mdk::NetHost &host, msg::Buffer &buf); //开始城市建设
	void OnCityBuilding(mdk::NetHost &host, msg::Buffer &buf); //为城市建设添砖加瓦
	void OnDelHouse(mdk::NetHost &host, msg::Buffer &buf); //地图技能：瓦割，拆房子

private:
	friend int main(int argc, char* argv[]);
	bool SendNotify(mdk::uint32 recverId, unsigned char recvType, msg::Buffer *pMsg, time_t holdTime = -1, mdk::uint32 sender = 0, const std::string &senderName = "系统");
	//结点信息
	class ConnectInfo : public mdk::HostData
	{
	public:
		mdk::int32			nodeId;//结点id
		Moudle::Moudle		type;//结点类型
		NetLine::NetLine	lineType;//运营商线路
		std::string			ip;//地址
		mdk::int32			port;//端口
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

	//业务属性
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