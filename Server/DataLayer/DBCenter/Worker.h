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

#include "Protocl/cpp/Object/Game/SetupVersion.h"
#include "Protocl/cpp/Object/Game/RaceMap.h"
#include "Protocl/cpp/Object/Game/SkillBook.h"
#include "Protocl/cpp/Object/Game/ItemBook.h"
#include "Protocl/cpp/Object/Game/TalentBook.h"
#include "Protocl/cpp/Object/Game/BuddyBook.h"
#include "Protocl/cpp/Object/Game/BuddyMap.h"
#include "Protocl/cpp/Object/Game/GetPlayerData.h"
#include "Protocl/cpp/Object/Game/Pets.h"
#include "Protocl/cpp/Object/Game/PlayerItems.h"

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
	 * ���ܣ���Ϣ������
	 * ˵�����ô������˵�����Ҫ����֮�����Ϣ������¼Log
	 */
	virtual void OnMsg(mdk::STNetHost& host);
	void OnAuth(mdk::STNetHost &host, msg::Buffer &buffer);//��֤ģ����Ϣ��Ӧ
	void OnSNS(mdk::STNetHost &host, msg::Buffer &buffer);//�罻ģ����Ϣ��Ӧ
	void OnGame(mdk::STNetHost &host, msg::Buffer &buffer);//��Ϸģ�Ὺ��Ϣ��Ӧ
	//////////////////////////////////////////////////////////////////////////
	// ��֤��ģ��
	//////////////////////////////////////////////////////////////////////////
	// �û�ע����Ϣ
	bool OnUserRegister(mdk::STNetHost& host, msg::Buffer& buffer);
	// �û���������
	bool OnResetPassword(mdk::STNetHost& host, msg::Buffer& buffer);
	// �û����ֻ�����
	bool OnBindingPhone(mdk::STNetHost& host, msg::Buffer& buffer);
	//////////////////////////////////////////////////////////////////////////
	//SNSģ��
	//��ӻ��
	void OnAddBuddy(mdk::STNetHost &host, msg::Buffer &buffer);
	void OnDelBuddy(mdk::STNetHost &host, msg::Buffer &buffer);
	//�����û�����
	void OnSetUserData(mdk::STNetHost &host, msg::Buffer &buffer);
	//////////////////////////////////////////////////////////////////////////
	//��Ϸģ��
	//��װ�汾����
	bool OnSetupVersion(mdk::STNetHost &host, msg::Buffer &buffer);
	void OnGetPlayerData(mdk::STNetHost &host, msg::Buffer &buffer);


private:
	friend int main(int argc, char* argv[]);
	bool InitId( const std::string &fileName );
	bool CreateId( mdk::uint32 &id, bool now = true );
	void SaveId( bool now );

	// �����û���������ע��Э������ݣ�����û���Ϣ
	bool CreateUser(msg::UserRegister& userRegister, Cache::User& user);
	// ����һ���û�ID�����������û�ʱ������ȫ��Ψһ��userId
	bool CreateUserId(int& userId);
	// ����һ��32���ֽڵ�randKey�����������û�ʱ�����user�ṹ�е�randKey�ֶ�
	void RandKey(std::string& randKey);
	// ����һ���û����ǳƣ����������û�ʱ����Ĭ�����
	void RandNickname(std::string& nickname, int userId);
	bool SetBuddy(mdk::uint32 userid, mdk::uint32 buddyId, msg::AddBuddy &msg);
	bool DelBuddy(mdk::uint32 userId, mdk::uint32 buddyId, msg::DelBuddy &msg);
	bool SetUserData(msg::SetUserData &msg);

	//�������
	bool CreatePlayer(unsigned int userId);
	//��ȡ��������
	bool ReadSkill(MySqlClient *pMysql, std::vector<data::SKILL> &skills, ResultCode::ResultCode &result, std::string &reason);
	//��ȡ��Ʒ����
	bool ReadItem(MySqlClient *pMysql, std::vector<data::ITEM> &imtes, ResultCode::ResultCode &result, std::string &reason);
	//��ȡ��������
	bool ReadTalent(MySqlClient *pMysql, std::vector<data::TALENT> &talents, ResultCode::ResultCode &result, std::string &reason);
	//��ȡ�͵�������
	bool ReadBuddy(MySqlClient *pMysql, std::vector<data::BUDDY> &buddys, ResultCode::ResultCode &result, std::string &reason);
	//ȡ�͵��޵������� 
	bool ReadBuddyLBS(MySqlClient *pMysql, std::vector<data::BUDDY_MAP> &buddyMaps, ResultCode::ResultCode &result, std::string &reason);
	//ȡ����
	bool ReadPets(MySqlClient *pMysql, unsigned int userId, std::vector<data::PET> &pets, ResultCode::ResultCode &result, std::string &reason);
	//ȡ�����Ʒ 
	bool ReadPlayerItems(MySqlClient *pMysql, unsigned int userId, std::vector<data::PLAYER_ITEM> &items, ResultCode::ResultCode &result, std::string &reason);
private:
	mdk::Logger     m_log;
	mdk::ConfigFile m_cfg;
	mdk::uint16 m_nodeId;
	mdk::uint32 m_searialNo;
	mdk::ConfigFile *m_pCfg;

	CacheInterface  m_cache;
	MysqlCluster    m_mySQLCluster;
	ClusterMgr      m_cluster;
};

#endif // __WORKER_H__