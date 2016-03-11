#ifndef GAME_H
#define GAME_H

#include <ctime>
#include "common/BuddyData.h"

class Game
{
public:
	Game(void);
	virtual ~Game(void);
	bool IsInit();
	int Version();

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
	short Encounter( int mapId );

private:
	bool LoadGameInit();
	bool SaveGameInit();

private:
	bool									m_gameInitLoaded;
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
};

#endif //GAME_H