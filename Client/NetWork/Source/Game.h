#ifndef GAME_H
#define GAME_H

#include <ctime>
#include "common/BuddyData.h"
#include "Battle.h"
#include "BuddyWorld.h"

class Game
{
	Battle m_asda;
	friend class Battle;
public:
	Game(void);
	virtual ~Game(void);
	bool IsInit();
	int Version();
	std::map<unsigned char, std::string>& RaceBook();
	std::vector<data::ITEM>& ItemBook();
	std::vector<data::TALENT>& TalentBook();
	std::vector<data::SKILL>& SkillBook();
	std::vector<data::BUDDY>& BuddyBook();
	std::vector<data::BUDDY_MAP>& BuddyMaps();
	data::SKILL* BornSkill();
	data::ITEM* NullItem(); 

	void SetRaceMap(std::map<unsigned char, std::string> &raceBook);
	void SetItemBook(std::vector<data::ITEM> &itemBook);
	void SetTalentBook(std::vector<data::TALENT> &talentBook);
	void SetSkillBook(std::vector<data::SKILL> &skillBook);
	void SetBuddyBook(std::vector<data::BUDDY> &buddyBook);
	void SetBuddyMap(std::vector<data::BUDDY_MAP> &buddyMaps);
	void OnSetupVersion(unsigned short dataVersion);

	std::string TestLuck(short &luckCoin, short &itemId);
	std::string Buy( short itemId, int count, int &coin );
	std::string Devour( short itemId, int count, int &coin );
	data::BUDDY* Encounter( int mapId );
	//////////////////////////////////////////////////////////////////////////
	//战斗接口
	int CreateBattle(unsigned int mePlayerId, const std::string &playerName, 
		std::vector<data::PET> &me, unsigned int shePlayerId, 
		const std::string &enemyName, std::vector<data::PET> &she);

	int CreateBattle(unsigned int mePlayerId, 
		const std::string &playerName, std::vector<data::PET> &me);
	int LoadBattle();
	bool Log( int battleId, std::vector<std::string> &log );
	//产生本地随机数
	void CreateRP(int battleId, bool me, Battle::RAND_PARAM &rp);
	//操作完成，已准备
	const char* Ready(int battleId, bool me, Battle::Action act, 
						short objectId, Battle::RAND_PARAM &rp);
	const char* ChangePet(int battleId, bool me, short petId);//更换巴迪

	Battle* GetBattle(int battleId);
	Battle::WARRIOR* Fighter(int battleId, bool me);
	bool AutoRound( int battleId );

private:
	bool LoadGameInit();
	bool SaveGameInit();


private:
	bool									m_gameInitLoaded;
	data::SKILL								m_bornSkill;//默认技能
	data::ITEM								m_nullItem;//
	//当前版本数据
	int										m_gameInitVersion;
	std::map<unsigned char, std::string>	m_raceBook;
	std::vector<data::ITEM>					m_itemBook;
	std::vector<data::TALENT>				m_talentBook;
	std::vector<data::SKILL>				m_skillBook;
	std::vector<data::BUDDY>				m_buddyBook;
	std::vector<data::BUDDY_MAP>			m_buddyMaps;

	//更新版本数据
	std::map<unsigned char, std::string>	m_raceBookNew;
	std::vector<data::ITEM>					m_itemBookNew;
	std::vector<data::TALENT>				m_talentBookNew;
	std::vector<data::SKILL>				m_skillBookNew;
	std::vector<data::BUDDY>				m_buddyBookNew;
	std::vector<data::BUDDY_MAP>			m_buddyMapsNew;

	time_t m_lastBattleTime;
	std::map<int, Battle>	m_battles;

	//npc
	BuddyWorld	m_buddyWorld;
};

#endif //GAME_H