#ifndef CLIENT_H
#define CLIENT_H

#include "NetWorker.h"
#include "Game.h"
#include "mdk/File.h"

class Client : public NetWorker
{
public:
	Client(void);
	virtual ~Client(void);
	enum SvrType
	{
		TcpSvr = 0,
	};

public:
	//////////////////////////////////////////////////////////////////////////
	//Auth
	bool Register(bool isMobile, const std::string &account, const std::string &pwd);
	bool Login(bool isMobile, const std::string &account, const std::string &pwd);
	bool BindPhone(const std::string &moblie);
	bool SetPassword(const std::string pwd);
	//////////////////////////////////////////////////////////////////////////
	//SNS
	bool GetUserData(unsigned int userId);
	bool GetBuddys();
	bool GetEvent();
	bool AddBuddy(unsigned int buddyId, const std::string &talk);
	bool AcceptBuddy(unsigned int buddyId, const std::string &talk, bool accept);
	bool DelBuddy( unsigned int buddyId );
	bool Chat(unsigned int recverId, unsigned char recvType, const std::string &talk);
	bool SetUserData(unsigned int userId);

	//////////////////////////////////////////////////////////////////////////
	//Game
	void SetLBS(int mapId);
	std::string TestLuck();//试手气（摇钱树）
	std::string UseItem( short itemId, int count );//使用物品
	std::string Buy( short itemId, int count );//购买物品
	std::string Devour( short itemId, int count );//吞噬物品
	//战斗接口
	int CreateBattle( unsigned int shePlayerId, const std::string &enemyName,
		std::vector<data::PET> &she);//对战
	int CreateBattle();//野战
	bool Log( int battleId, std::vector<std::string> &log );
	//产生本地随机数
	void CreateRP(int battleId, bool me, Battle::RAND_PARAM &rp);
	const char* Ready(int battleId, Battle::Action act, short objectId, Battle::RAND_PARAM &rp);//操作完成，准备
	const char* ChangePet(int battleId, short petId);//更换巴迪
	//对方行动
	const char* SheReady(int battleId, Battle::Action act, short objectId, Battle::RAND_PARAM &rp);
	const char* SheChangePet(int battleId, short petId);

	Game* GetGame();
	Battle* GetBattle(int battleId);
	Battle::WARRIOR* Fighter(int battleId, bool me);
	int LoadBattle();

protected:
	virtual void Main();
	virtual void OnConnect(int svrType, net::Socket &svr);
	virtual void OnClose(int svrType);
	virtual void OnMsg(int svrType, net::Socket &svr, msg::Buffer &buffer);

	void OnAuth(msg::Buffer &buffer);
	void OnSNS(msg::Buffer &buffer);
	void OnDBEntry(msg::Buffer &buffer);


	//////////////////////////////////////////////////////////////////////////
	//Auth
	void OnRegister(msg::Buffer &buffer);
	void OnLogin(msg::Buffer &buffer);
	void OnBindPhone(msg::Buffer &buffer);
	void OnRelogin(msg::Buffer &buffer);
	void OnResetPassword(msg::Buffer &buffer);

	//////////////////////////////////////////////////////////////////////////
	//SNS
	void OnAddBuddy(msg::Buffer &buffer);
	void OnDelBuddy(msg::Buffer &buffer);
	void OnBuddys(msg::Buffer &buffer);
	void OnChat(msg::Buffer &buffer);
	void OnUserData(msg::Buffer &buffer);

	void ClientInfo();

	//////////////////////////////////////////////////////////////////////////
	//game data tooler
	bool SaveGame();
	bool LoadGame();
	bool SaveItems(mdk::File &db, std::vector<data::PLAYER_ITEM> &items);
	int LoadItems(mdk::File &db, std::vector<data::PLAYER_ITEM> &items);
	bool GameSaved();
	void SyncGame();
	void IOCoin( int count );
	void IOItem( short itemId, int count );
	//////////////////////////////////////////////////////////////////////////
	//DBEntry
	void OnRaceMap(msg::Buffer &buffer);
	void OnItemBook(msg::Buffer &buffer);
	void OnTalentBook(msg::Buffer &buffer);
	void OnSkillBook(msg::Buffer &buffer);
	void OnBuddyBook(msg::Buffer &buffer);
	void OnBuddyMap(msg::Buffer &buffer);
	void OnSetupVersion(msg::Buffer &buffer);
	void OnPlayer(msg::Buffer &buffer);
	void OnPlayerItems(msg::Buffer &buffer);
	void OnPets(msg::Buffer &buffer);
	void OnGetPlayerData(msg::Buffer &buffer);
	void OnSyncPlayer(msg::Buffer &buffer);
	void OnSyncItem(msg::Buffer &buffer);
	void OnSyncPets(msg::Buffer &buffer);


private:
	typedef struct BUDDY_DATA
	{
		unsigned int id;
		std::string face;
		std::string nickName;
	}BUDDY_DATA;
	typedef struct USER_DATA
	{
		bool			logined;
		unsigned int	id;
		bool			isMobileLogin;
		std::string		account;
		std::string		pwd;
		std::string		newPwd;
		std::map<mdk::uint32, BUDDY_DATA>	buddys;
	}USER_DATA;

	net::Socket m_tcpEntry;
	time_t m_lastQueryTime;
	USER_DATA	m_user;

	Game							m_game;
	//玩家数据
	bool							m_palyerDataLoaded;
	data::PLAYER					m_player;
	std::vector<data::PLAYER_ITEM>	m_items;
	std::vector<data::PET>			m_pets;

	enum PlayerState
	{
		idle = 0,
		fight = 1,
	};
	PlayerState						m_state;
	int								m_mapId;
};

#endif //CLIENT_H