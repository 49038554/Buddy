#ifndef GAME_H
#define GAME_H

#include <ctime>
#include "common/BuddyData.h"
#include "Battle.h"

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
	int CreateBattle(unsigned int mePlayerId, unsigned int shePlayerId, 
		const std::string &playerName, const std::string &enemyName,
		std::vector<data::PET*> &me, std::vector<data::PET*> &she);
	const char* PlayerRand(int battleId, bool me, Battle::Action act, short objectId, Battle::RAND_PARAM &rp);
	
	bool PlayerAction(int battleId, bool me, Battle::Action act, short objectId, Battle::RAND_PARAM &rp);

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
};

#endif //GAME_H