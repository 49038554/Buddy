#ifndef __WORKER_H__
#define __WORKER_H__

#include "mdk/Logger.h"
#include "mdk/ConfigFile.h"
#include "frame/netserver/STNetHost.h"
#include "frame/netserver/STNetServer.h"
#include "Protocl/cpp/Object/Auth/UserRegister.h"
#include "DBCenter/CacheInterface.h"
#include "common/MysqlCluster.h"
#include "Interface/ClusterMgr/cpp/ClusterMgr.h"
#include "Protocl/cpp/Object/SNS/AddBuddy.h"
#include "Protocl/cpp/Object/SNS/DelBuddy.h"
#include "Protocl/cpp/Object/SNS/SetUserData.h"


/**
 * Worker
 */
class Worker : public mdk::STNetServer
{
public:
	Worker(void);
	virtual ~Worker(void);
	
protected:
	virtual void OnConnect(mdk::STNetHost &host);
	virtual void OnCloseConnect(mdk::STNetHost &host);
	/**
	 * 功能：消息处理函数
	 * 说明：该处理会过滤掉除了要处理之外的消息，并记录Log
	 */
	virtual void OnMsg(mdk::STNetHost& host);
	void OnAuth(mdk::STNetHost &host, msg::Buffer &buffer);//认证模块消息响应
	void OnSNS(mdk::STNetHost &host, msg::Buffer &buffer);//社交模块消息响应
	void OnDBEntry(mdk::STNetHost &host, msg::Buffer &buffer);//数据模块消息响应
	//////////////////////////////////////////////////////////////////////////
	// 认证服模块
	//////////////////////////////////////////////////////////////////////////
	// 用户注册消息
	bool OnUserRegister(mdk::STNetHost& host, msg::Buffer& buffer);
	// 用户重设密码
	bool OnResetPassword(mdk::STNetHost& host, msg::Buffer& buffer);
	// 用户绑定手机号码
	bool OnBindingPhone(mdk::STNetHost& host, msg::Buffer& buffer);
	//////////////////////////////////////////////////////////////////////////
	//SNS模块
	//添加伙伴
	void OnAddBuddy(mdk::STNetHost &host, msg::Buffer &buffer);
	void OnDelBuddy(mdk::STNetHost &host, msg::Buffer &buffer);
	//设置用户数据
	void OnSetUserData(mdk::STNetHost &host, msg::Buffer &buffer);
	//////////////////////////////////////////////////////////////////////////
	//游戏模块
	//安装版本数据
	bool OnSetupVersion(mdk::STNetHost &host, msg::Buffer &buffer);
	void OnGetPlayerData(mdk::STNetHost &host, msg::Buffer &buffer);
	void OnBuildHouse(mdk::STNetHost &host, msg::Buffer &buffer);//地图技能：光墙，造房子
	void OnTreePlant(mdk::STNetHost &host, msg::Buffer &buffer);//种树
	void OnSyncItem(mdk::STNetHost &host, msg::Buffer &buffer);//同步物品
	void OnSyncPlayer(mdk::STNetHost &host, msg::Buffer &buffer);//同步正能量
	void OnSyncPets(mdk::STNetHost &host, msg::Buffer &buffer);//同步宠物,只添加不减少

private:
	friend int main(int argc, char* argv[]);
	bool InitId( const std::string &fileName );
	bool CreateId( mdk::uint32 &id, bool now = true );
	void SaveId( bool now );

	//加载初始化数据
	bool LoadGameInit();
	// 创建用户，并根据注册协议的内容，填充用户信息
	bool CreateUser(msg::UserRegister& userRegister, Cache::User& user);
	// 创建一个用户ID，用来创建用户时，生成全局唯一的userId
	bool CreateUserId(int& userId);
	// 创建一个32个字节的randKey，用来创建用户时，填充user结构中的randKey字段
	void RandKey(std::string& randKey);
	// 创建一个用户的昵称，用来创建用户时进行默认填充
	void RandNickname(std::string& nickname, int userId);
	bool SetBuddy(mdk::uint32 userid, mdk::uint32 buddyId, msg::AddBuddy &msg);
	bool DelBuddy(mdk::uint32 userId, mdk::uint32 buddyId, msg::DelBuddy &msg);
	bool SetUserData(msg::SetUserData &msg);

	//创建玩家
	bool CreatePlayer(unsigned int userId);
	//读取物品数据
	bool ReadItem(MySqlClient *pMysql, std::vector<data::ITEM> &imtes);
	//读取特性数据
	bool ReadTalent(MySqlClient *pMysql, std::vector<data::TALENT> &talents);
	//读取技能数据
	bool ReadSkill(MySqlClient *pMysql, std::vector<data::SKILL> &skills);
	//读取巴迪兽数据
	bool ReadBuddy(MySqlClient *pMysql, std::vector<data::BUDDY> &buddys);
	//取巴迪兽地理数据 
	bool ReadBuddyLBS(MySqlClient *pMysql, std::vector<data::BUDDY_MAP> &buddyMaps);
	//取证能量
	bool ReadPlayer(MySqlClient *pMysql, data::PLAYER &player, ResultCode::ResultCode &result, std::string &reason);
	//取宠物
	bool ReadPets(MySqlClient *pMysql, unsigned int userId, std::vector<data::PET> &pets, ResultCode::ResultCode &result, std::string &reason);
	//取玩家物品 
	bool ReadPlayerItems(MySqlClient *pMysql, unsigned int userId, std::vector<data::PLAYER_ITEM> &items, ResultCode::ResultCode &result, std::string &reason);

	//////////////////////////////////////////////////////////////////////////
	//Game
	//添加房子
	int AddHouse(unsigned int owner, const std::string &name, const std::string &address,
		const std::string &longitude, const std::string &latitude,
		int	radius, int coin );
	//添加果树
	int AddTree(unsigned int owner, int houseId );
	bool UseItem(unsigned int userId, int itemId, int count);
	bool SyncItem(unsigned int userId, int itemId, int count);
	bool SyncPlayer(data::PLAYER &player);
	bool SyncPet( unsigned int userId, int petId, 
		int number, char talent, char nature, char SDHealthy,
		unsigned char HPMuscle, unsigned char WGMuscle, unsigned char WFMuscle, unsigned char TGMuscle, unsigned char TFMuscle, unsigned char SDMuscle,
		std::vector<char> race );
	bool AddPet( int number, unsigned userId, int petId, char talent, char nature, char SDHealthy);

private:
	mdk::Logger     m_log;
	mdk::ConfigFile m_cfg;
	mdk::uint16 m_nodeId;
	mdk::uint32 m_searialNo;
	mdk::ConfigFile *m_pCfg;

	CacheInterface  m_cache;
	MysqlCluster    m_mySQLCluster;
	ClusterMgr      m_cluster;

	int m_gameVersion;
	std::map<unsigned char, std::string>	m_races;
	std::vector<data::SKILL>	m_skills;
	std::vector<data::ITEM>		m_items;
	std::vector<data::TALENT>	m_talents;
	std::vector<data::BUDDY>	m_buddys;
	std::vector<data::BUDDY_MAP> m_buddyMaps;
};

#endif // __WORKER_H__