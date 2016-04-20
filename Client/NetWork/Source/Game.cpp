#include "Game.h"

#include "mdk/File.h"

Game::Game(void)
{
	m_gameInitVersion = 0;
	m_gameInitLoaded = LoadGameInit();
	m_lastBattleTime = time(NULL);

	m_bornSkill.id = 0;
	m_bornSkill.name = "肉搏";
	m_bornSkill.race = Race::pu;//属性
	m_bornSkill.power = 60;//威力0~300
	m_bornSkill.type = 1;//1物理，2变化，3特殊
	m_bornSkill.hitRate = 100;//命中率30~101, 101必中技
	m_bornSkill.isMapSkill = false;//是地图技能
	m_bornSkill.descript  = "无技能可用时使用的技能";//最大60byte

	m_nullItem.id = 0;
	m_nullItem.name = "空";
	m_buddyWorld.SetGame(this);
}

Game::~Game(void)
{
}

bool Game::IsInit()
{
	return m_gameInitLoaded;
}

int Game::Version()
{
	return m_gameInitVersion;
}

std::map<unsigned char, std::string>& Game::RaceBook()
{
	return m_raceBook;
}

std::vector<data::ITEM>& Game::ItemBook()
{
	return m_itemBook;
}

std::vector<data::TALENT>& Game::TalentBook()
{
	return m_talentBook;
}

std::vector<data::SKILL>& Game::SkillBook()
{
	return m_skillBook;
}

std::vector<data::BUDDY>& Game::BuddyBook()
{
	return m_buddyBook;
}

std::vector<data::BUDDY_MAP>& Game::BuddyMaps()
{
	return m_buddyMaps;
}

data::SKILL* Game::BornSkill()
{
	return &m_bornSkill;
}

data::ITEM* Game::NullItem()
{
	return &m_nullItem;
}

bool SaveRaceBook( mdk::File &db, std::map<unsigned char, std::string> &races )
{
	std::map<unsigned char, std::string>::iterator it = races.begin();
	unsigned char len = races.size();
	db.Write(&len, sizeof(char));
	unsigned char raceId;
	for ( ; it != races.end(); it++ )
	{
		raceId = it->first;
		db.Write(&raceId, 1);
		len = it->second.size();
		db.Write(&len, sizeof(char));
		db.Write((char*)(it->second.c_str()), len);
	}

	return true;
}

int LoadRaceBook( mdk::File &db, std::map<unsigned char, std::string> &races )
{
	races.clear();
	std::map<unsigned char, std::string>::iterator it = races.begin();
	unsigned char len = 0;
	unsigned char count = 0;
	if ( mdk::File::success != db.Read(&count, sizeof(char)) ) return 1;
	if ( 0 >= count || 17 < count ) return 2;
	unsigned char raceId;
	int i = 0;
	char buf[256];
	for ( i = 0; i < count; i++ )
	{
		if ( mdk::File::success != db.Read(&raceId, 1) ) return 3;
		if ( mdk::File::success != db.Read(&len, sizeof(char)) ) return 4;
		if ( 20 < len || 0 >= len ) return 5;
		if ( mdk::File::success != db.Read(buf, len) ) return 6;
		races[raceId] = std::string(buf, len);
	}

	return 0;
}

bool SaveItemBook( mdk::File &db, std::vector<data::ITEM> &items )
{
	data::ITEM *pItem;
	data::EFFECT *pEffect;
	char len;
	short count = items.size();
	db.Write(&count, sizeof(short));
	int i = 0;
	for ( i = 0; i < items.size(); i++ )
	{
		pItem = &items[i];

		db.Write(&pItem->id, sizeof(short));
		len = pItem->name.size();
		db.Write(&len, sizeof(char));
		db.Write((char*)(pItem->name.c_str()), len);
		db.Write(&pItem->coin, sizeof(int));
		len = pItem->descript.size();
		db.Write(&len, sizeof(char));
		db.Write(&pItem->descript, len);

		len = pItem->effects.size();
		db.Write(&len, sizeof(char));
		int j = 0;
		for ( j = 0; j < pItem->effects.size(); j++ )
		{
			pEffect = &pItem->effects[j];

			db.Write(&pEffect->id, sizeof(char));
			db.Write(&pEffect->step, sizeof(char));
			db.Write(&pEffect->probability, sizeof(char));
			db.Write(&pEffect->agent, sizeof(char));
		}

	}

	return true;
}

int LoadItemBook( mdk::File &db, std::vector<data::ITEM> &items )
{
	items.clear();
	data::ITEM info;
	data::EFFECT effect;
	char len;
	short count = 0;
	if ( mdk::File::success != db.Read(&count, sizeof(short)) ) return 1;
	if ( 0 >= count ) return 2;
	int i = 0;
	char buf[256];
	for ( i = 0; i < count; i++ )
	{
		if ( mdk::File::success != db.Read(&info.id, sizeof(short)) ) return 3;
		if ( mdk::File::success != db.Read(&len, sizeof(char)) ) return 4;
		if ( 20 < len || 0 >= len ) return 5;
		if ( mdk::File::success != db.Read(buf, len) ) return 6;
		info.name = std::string(buf, len);
		if ( mdk::File::success != db.Read(&info.coin, sizeof(int)) ) return 7;
		if ( mdk::File::success != db.Read(&len, sizeof(char)) ) return 8;
		if ( 60 < len || 0 >= len ) return 9;
		if ( mdk::File::success != db.Read(&buf, len) ) return 10;
		info.descript = std::string(buf, len);

		if ( mdk::File::success != db.Read(&len, sizeof(char)) ) return 11;
		if ( 5 < len || 0 > len ) return 12;
		int j = 0;
		info.effects.clear();
		for ( j = 0; j < len; j++ )
		{
			if ( mdk::File::success != db.Read(&effect.id, sizeof(char)) ) return 13;
			if ( mdk::File::success != db.Read(&effect.step, sizeof(char)) ) return 14;
			if ( mdk::File::success != db.Read(&effect.probability, sizeof(char)) ) return 15;
			if ( mdk::File::success != db.Read(&effect.agent, sizeof(char)) ) return 16;
			info.effects.push_back(effect);
		}

		items.push_back(info);
	}

	return 0;
}

bool SaveTalentBook(mdk::File &db, std::vector<data::TALENT> &talents)
{
	data::TALENT *pTalent;
	data::EFFECT *pEffect;
	char len;
	short count = talents.size();
	db.Write(&count, sizeof(short));
	int i = 0;
	for ( i = 0; i < talents.size(); i++ )
	{
		pTalent = &talents[i];

		db.Write(&pTalent->id, sizeof(short));
		len = pTalent->name.size();
		db.Write(&len, sizeof(char));
		db.Write((char*)(pTalent->name.c_str()), len);
		len = pTalent->descript.size();
		db.Write(&len, sizeof(char));
		db.Write(&pTalent->descript, len);

		len = pTalent->effects.size();
		db.Write(&len, sizeof(char));
		int j = 0;
		for ( j = 0; j < pTalent->effects.size(); j++ )
		{
			pEffect = &pTalent->effects[j];

			db.Write(&pEffect->id, sizeof(char));
			db.Write(&pEffect->step, sizeof(char));
			db.Write(&pEffect->probability, sizeof(char));
			db.Write(&pEffect->agent, sizeof(char));
		}

	}

	return true;
}

int LoadTalentBook(mdk::File &db, std::vector<data::TALENT> &talents)
{
	talents.clear();
	data::TALENT info;
	data::EFFECT effect;
	char len;
	short count = 0;
	if ( mdk::File::success != db.Read(&count, sizeof(short)) ) return 1;
	if ( 0 >= count ) return 2;
	int i = 0;
	char buf[256];
	for ( i = 0; i < count; i++ )
	{
		if ( mdk::File::success != db.Read(&info.id, sizeof(short)) ) return 3;
		if ( mdk::File::success != db.Read(&len, sizeof(char)) ) return 4;
		if ( 20 < len || 0 >= len ) return 5;
		if ( mdk::File::success != db.Read(buf, len) ) return 6;
		info.name = std::string(buf, len);
		if ( mdk::File::success != db.Read(&len, sizeof(char)) ) return 7;
		if ( 60 < len || 0 >= len ) return 8;
		if ( mdk::File::success != db.Read(&buf, len) ) return 9;
		info.descript = std::string(buf, len);

		if ( mdk::File::success != db.Read(&len, sizeof(char)) ) return 10;
		if ( 5 < len || 0 >= len ) return 11;
		int j = 0;
		info.effects.clear();
		for ( j = 0; j < len; j++ )
		{
			if ( mdk::File::success != db.Read(&effect.id, sizeof(char)) ) return 12;
			if ( mdk::File::success != db.Read(&effect.step, sizeof(char)) ) return 13;
			if ( mdk::File::success != db.Read(&effect.probability, sizeof(char)) ) return 14;
			if ( mdk::File::success != db.Read(&effect.agent, sizeof(char)) ) return 15;
			info.effects.push_back(effect);
		}
		talents.push_back(info);
	}

	return 0;
}

bool SaveSkillBook(mdk::File &db, std::vector<data::SKILL> &skills)
{
	data::SKILL *pSkill;
	data::EFFECT *pEffect;
	char len;
	short count = skills.size();
	db.Write(&count, sizeof(short));
	int i = 0;
	char valChar;
	for ( i = 0; i < skills.size(); i++ )
	{
		pSkill = &skills[i];

		db.Write(&pSkill->id, sizeof(short));
		len = pSkill->name.size();
		db.Write(&len, sizeof(char));
		db.Write((char*)(pSkill->name.c_str()), len);
		len = pSkill->descript.size();
		db.Write(&len, sizeof(char));
		db.Write(&pSkill->descript, len);
		db.Write(&pSkill->race, sizeof(char));
		db.Write(&pSkill->power, sizeof(short));
		db.Write(&pSkill->type, sizeof(char));
		db.Write(&pSkill->hitRate, sizeof(char));
		valChar = pSkill->isMapSkill?1:0;
		db.Write(&valChar, sizeof(char));

		len = pSkill->effects.size();
		db.Write(&len, sizeof(char));
		int j = 0;
		for ( j = 0; j < pSkill->effects.size(); j++ )
		{
			pEffect = &pSkill->effects[j];

			db.Write(&pEffect->id, sizeof(char));
			db.Write(&pEffect->step, sizeof(char));
			db.Write(&pEffect->probability, sizeof(char));
			db.Write(&pEffect->agent, sizeof(char));
		}

	}

	return true;
}

int LoadSkillBook(mdk::File &db, std::vector<data::SKILL> &skills)
{
	skills.clear();
	data::SKILL info;
	data::EFFECT effect;
	char len;
	short count = 0;
	if ( mdk::File::success != db.Read(&count, sizeof(short)) ) return 1;
	if ( 0 >= count ) return 2;
	int i = 0;
	char valChar;
	char buf[256];
	for ( i = 0; i < count; i++ )
	{
		if ( mdk::File::success != db.Read(&info.id, sizeof(short)) ) return 3;
		if ( mdk::File::success != db.Read(&len, sizeof(char)) ) return 4;
		if ( 20 < len || 0 >= len ) return 5;
		if ( mdk::File::success != db.Read(buf, len) ) return 6;
		info.name = std::string(buf, len);
		if ( mdk::File::success != db.Read(&len, sizeof(char)) ) return 7;
		if ( 60 < len || 0 >= len ) return 8;
		if ( mdk::File::success != db.Read(&buf, len) ) return 9;
		info.descript = std::string(buf, len);
		if ( mdk::File::success != db.Read(&info.race, sizeof(char)) ) return 10;
		if ( mdk::File::success != db.Read(&info.power, sizeof(short)) ) return 11;
		if ( mdk::File::success != db.Read(&info.type, sizeof(char)) ) return 12;
		if ( mdk::File::success != db.Read(&info.hitRate, sizeof(char)) ) return 13;
		if ( mdk::File::success != db.Read(&valChar, sizeof(char)) ) return 14;
		info.isMapSkill = (0 == valChar?false:true);

		if ( mdk::File::success != db.Read(&len, sizeof(char)) ) return 15;
		if ( 5 < len || 0 > len ) return 16;
		int j = 0;
		info.effects.clear();
		for ( j = 0; j < len; j++ )
		{
			if ( mdk::File::success != db.Read(&effect.id, sizeof(char)) ) return 17;
			if ( mdk::File::success != db.Read(&effect.step, sizeof(char)) ) return 18;
			if ( mdk::File::success != db.Read(&effect.probability, sizeof(char)) ) return 19;
			if ( mdk::File::success != db.Read(&effect.agent, sizeof(char)) ) return 20;

			info.effects.push_back(effect);
		}

		skills.push_back(info);
	}

	return 0;
}

bool SaveBuddyBook(mdk::File &db, std::vector<data::BUDDY> &buddys)
{
	data::BUDDY *pBuddy;
	char len;
	short count = buddys.size();
	db.Write(&count, sizeof(short));
	int i = 0;
	for ( i = 0; i < buddys.size(); i++ )
	{
		pBuddy = &buddys[i];

		db.Write(&pBuddy->number, sizeof(short));
		len = pBuddy->name.size();
		db.Write(&len, sizeof(char));
		db.Write((char*)(pBuddy->name.c_str()), len);
		len = pBuddy->descript.size();
		db.Write(&len, sizeof(char));
		db.Write(&pBuddy->descript, len);
		db.Write(&pBuddy->race1, sizeof(char));
		db.Write(&pBuddy->race2, sizeof(char));
		db.Write(&pBuddy->talent1, sizeof(char));
		db.Write(&pBuddy->talent2, sizeof(char));
		db.Write(&pBuddy->talent3, sizeof(char));
		db.Write(&pBuddy->itemId, sizeof(short));
		db.Write(&pBuddy->hitPoint, sizeof(short));
		db.Write(&pBuddy->physicalA, sizeof(short));
		db.Write(&pBuddy->physicalD, sizeof(short));
		db.Write(&pBuddy->specialA, sizeof(short));
		db.Write(&pBuddy->specialD, sizeof(short));
		db.Write(&pBuddy->speed, sizeof(short));
		db.Write(&pBuddy->rare, sizeof(char));
		db.Write(&pBuddy->tame, sizeof(char));

		len = pBuddy->skills.size();
		db.Write(&len, sizeof(char));
		short valShort;
		char valChar;
		std::map<unsigned short, bool>::iterator it = pBuddy->skills.begin();
		for ( ; it != pBuddy->skills.end(); it++ )
		{
			valShort = it->first;
			valChar = it->second?1:0;
			db.Write(&valShort, sizeof(short));
			db.Write(&valChar, sizeof(char));
		}

		len = pBuddy->upBuddys.size();
		db.Write(&len, sizeof(char));
		int j = 0;
		for ( j = 0; j < pBuddy->upBuddys.size(); j++ )
		{
			valShort = pBuddy->upBuddys[j];
			db.Write(&valShort, sizeof(short));
		}

	}

	return true;
}

int LoadBuddyBook(mdk::File &db, std::vector<data::BUDDY> &buddys)
{
	buddys.clear();
	data::BUDDY info;
	char len;
	short count = buddys.size();
	if ( mdk::File::success != db.Read(&count, sizeof(short)) ) return 1;
	if ( 0 >= count ) return 2;

	int i = 0;
	char buf[256];
	for ( i = 0; i < count; i++ )
	{
		if ( mdk::File::success != db.Read(&info.number, sizeof(short)) ) return 3;
		if ( mdk::File::success != db.Read(&len, sizeof(char)) ) return 4;
		if ( 20 < len || 0 >= len ) return 5;
		if ( mdk::File::success != db.Read(buf, len) ) return 6;
		info.name = std::string(buf, len);
		if ( mdk::File::success != db.Read(&len, sizeof(char)) ) return 7;
		if ( 60 < len || 0 >= len ) return 8;
		if ( mdk::File::success != db.Read(buf, len) ) return 9;
		info.descript = std::string(buf, len);
		if ( mdk::File::success != db.Read(&info.race1, sizeof(char)) ) return 10;
		if ( mdk::File::success != db.Read(&info.race2, sizeof(char)) ) return 11;
		if ( mdk::File::success != db.Read(&info.talent1, sizeof(char)) ) return 12;
		if ( mdk::File::success != db.Read(&info.talent2, sizeof(char)) ) return 13;
		if ( mdk::File::success != db.Read(&info.talent3, sizeof(char)) ) return 14;
		if ( mdk::File::success != db.Read(&info.itemId, sizeof(short)) ) return 15;
		if ( mdk::File::success != db.Read(&info.hitPoint, sizeof(short)) ) return 16;
		if ( mdk::File::success != db.Read(&info.physicalA, sizeof(short)) ) return 17;
		if ( mdk::File::success != db.Read(&info.physicalD, sizeof(short)) ) return 18;
		if ( mdk::File::success != db.Read(&info.specialA, sizeof(short)) ) return 19;
		if ( mdk::File::success != db.Read(&info.specialD, sizeof(short)) ) return 20;
		if ( mdk::File::success != db.Read(&info.speed, sizeof(short)) ) return 21;
		if ( mdk::File::success != db.Read(&info.rare, sizeof(char)) ) return 22;
		if ( mdk::File::success != db.Read(&info.tame, sizeof(char)) ) return 23;

		len = 0;
		if ( mdk::File::success != db.Read(&len, sizeof(char)) ) return 24;
		if ( 80 < len || 0 >= len ) return 25;
		short valShort;
		char valChar;
		int j = 0;
		info.skills.clear();
		for ( ; j < len; j++ )
		{
			if ( mdk::File::success != db.Read(&valShort, sizeof(short)) ) return 26;
			if ( mdk::File::success != db.Read(&valChar, sizeof(char)) ) return 27;
			info.skills[valShort] = (0 == valChar?false:true); 
		}

		len = 0;
		if ( mdk::File::success != db.Read(&len, sizeof(char)) ) return 28;
		if ( 10 < len || 0 > len) return 29;
		info.upBuddys.clear();
		for ( j = 0; j < len; j++ )
		{
			if ( mdk::File::success != db.Read(&valShort, sizeof(short)) ) return 30;
			info.upBuddys.push_back(valShort);
		}
		buddys.push_back(info);
	}

	return 0;
}

bool SaveBuddyMap(mdk::File &db, std::vector<data::BUDDY_MAP> &buddyMaps)
{
	data::BUDDY_MAP *pBuddyMap;
	unsigned char len;
	short count = buddyMaps.size();
	db.Write(&count, sizeof(short));
	int i = 0;
	char valChar;
	short valShort;
	int j = 0;
	for ( i = 0; i < buddyMaps.size(); i++ )
	{
		pBuddyMap = &buddyMaps[i];

		db.Write(&pBuddyMap->id, sizeof(int));
		db.Write(&pBuddyMap->shape, sizeof(char));
		db.Write(&pBuddyMap->x, sizeof(double));//latitude
		db.Write(&pBuddyMap->y, sizeof(double));//longitude
		db.Write(&pBuddyMap->radius, sizeof(int));
		db.Write(&pBuddyMap->right, sizeof(double));//latitude
		db.Write(&pBuddyMap->bottom, sizeof(double));//longitude
		db.Write(&pBuddyMap->city, sizeof(int));
		valChar = pBuddyMap->spot?1:0;
		db.Write(&valChar, sizeof(char));

		len = pBuddyMap->buddys.size();
		db.Write(&len, sizeof(char));
		for ( j = 0; j < pBuddyMap->buddys.size(); j++ )
		{
			valShort = pBuddyMap->buddys[j];
			db.Write(&valShort, sizeof(short));
		}

	}

	return true;
}

int LoadBuddyMap(mdk::File &db, std::vector<data::BUDDY_MAP> &buddyMaps)
{
	buddyMaps.clear();
	data::BUDDY_MAP info;
	unsigned char len;
	short count = 0;
	if ( mdk::File::success != db.Read(&count, sizeof(short)) ) return 1;
	if ( 0 >= count ) return 2;

	int i = 0;
	char valChar;
	short valShort;
	for ( i = 0; i < count; i++ )
	{
		if ( mdk::File::success != db.Read(&info.id, sizeof(int)) ) return 3;
		if ( mdk::File::success != db.Read(&info.shape, sizeof(char)) ) return 4;
		if ( mdk::File::success != db.Read(&info.x, sizeof(double)) ) return 5;//latitude
		if ( mdk::File::success != db.Read(&info.y, sizeof(double)) ) return 6;//longitude
		if ( mdk::File::success != db.Read(&info.radius, sizeof(int)) ) return 7;
		if ( mdk::File::success != db.Read(&info.right, sizeof(double)) ) return 9;//latitude
		if ( mdk::File::success != db.Read(&info.bottom, sizeof(double)) ) return 10;//longitude
		if ( mdk::File::success != db.Read(&info.city, sizeof(int)) ) return 11;
		if ( mdk::File::success != db.Read(&valChar, sizeof(char)) ) return 12;
		info.spot = (0 == valChar?false:true);
		if ( mdk::File::success != db.Read(&len, sizeof(char)) ) return 13;
		if ( 100 < len || 0 >= len) return 14;
		int j = 0;
		info.buddys.clear();
		for ( j = 0; j < len; j++ )
		{
			if ( mdk::File::success != db.Read(&valShort, sizeof(short)) ) return 15;
			info.buddys.push_back(valShort);
		}
		buddyMaps.push_back(info);
	}

	return 0;
}

bool Game::LoadGameInit()
{
	if ( m_gameInitLoaded ) return true;

	mdk::File db("D:/data", "buddy.db");

	if ( mdk::File::success != db.Open(mdk::File::read, mdk::File::assii) ) return false;
	if ( mdk::File::success != db.Read(&m_gameInitVersion, sizeof(int)) ) return false;

	int ret = LoadRaceBook(db, m_raceBook);//
	if ( 0 != ret ) return false;
	ret = LoadItemBook(db, m_itemBook);//
	if ( 0 != ret ) return false;
	ret = LoadTalentBook(db, m_talentBook);//
	if ( 0 != ret ) return false;
	ret = LoadSkillBook(db, m_skillBook);//
	if ( 0 != ret ) return false;
	ret = LoadBuddyBook(db, m_buddyBook);//
	if ( 0 != ret ) return false;
	ret = LoadBuddyMap(db, m_buddyMaps);//
	if ( 0 != ret ) return false;

	return true;
}

bool Game::SaveGameInit()
{
	mdk::File db("D:/data", "buddy.db");

	if ( mdk::File::success != db.Open(mdk::File::write, mdk::File::assii) ) return false;
	db.Write(&m_gameInitVersion, sizeof(int));
	SaveRaceBook(db, m_raceBook);//
	SaveItemBook(db, m_itemBook);//
	SaveTalentBook(db, m_talentBook);//
	SaveSkillBook(db, m_skillBook);//
	SaveBuddyBook(db, m_buddyBook);//
	SaveBuddyMap(db, m_buddyMaps);//

	return true;
}

void Game::SetRaceMap(std::map<unsigned char, std::string> &raceBook)
{
	m_raceBookNew = raceBook;
}

void Game::SetItemBook(std::vector<data::ITEM> &itemBook)
{
	int i = 0;
	data::ITEM *pInfo;
	for ( i = 0; i < itemBook.size(); i++ )
	{
		pInfo = Item(itemBook[i].id, m_itemBookNew);
		if ( NULL == pInfo ) m_itemBookNew.push_back(itemBook[i]);
		else *pInfo = itemBook[i];
	}
}

void Game::SetTalentBook(std::vector<data::TALENT> &talentBook)
{
	int i = 0;
	data::TALENT *pInfo;
	data::TALENT info;
	for ( i = 0; i < talentBook.size(); i++ )
	{
		pInfo = Talent(talentBook[i].id, m_talentBookNew);
		if ( NULL == pInfo ) m_talentBookNew.push_back(talentBook[i]);
		else *pInfo = talentBook[i];
	}
}

void Game::SetSkillBook(std::vector<data::SKILL> &skillBook)
{
	int i = 0;
	data::SKILL *pInfo;
	for ( i = 0; i < skillBook.size(); i++ )
	{
		pInfo = Skill(skillBook[i].id, m_skillBookNew);
		if ( NULL == pInfo ) m_skillBookNew.push_back(skillBook[i]);
		else *pInfo = skillBook[i];
	}
}

void Game::SetBuddyBook(std::vector<data::BUDDY> &buddyBook)
{
	int i = 0;
	data::BUDDY *pInfo;
	for ( i = 0; i < buddyBook.size(); i++ )
	{
		pInfo = Buddy(buddyBook[i].number, m_buddyBookNew);
		if ( NULL == pInfo ) m_buddyBookNew.push_back(buddyBook[i]);
		else *pInfo = buddyBook[i];
	}
}

void Game::SetBuddyMap(std::vector<data::BUDDY_MAP> &buddyMaps)
{
	int i = 0;
	data::BUDDY_MAP *pInfo;
	for ( i = 0; i < buddyMaps.size(); i++ )
	{
		pInfo = BuddyMap(buddyMaps[i].id, m_buddyMapsNew);
		if ( NULL == pInfo ) m_buddyMapsNew.push_back(buddyMaps[i]);
		else *pInfo = buddyMaps[i];
	}
}

void Game::OnSetupVersion(unsigned short dataVersion)
{
	if ( m_gameInitVersion == dataVersion ) return;

	m_gameInitVersion = dataVersion;
	m_raceBook = m_raceBookNew;
	m_itemBook = m_itemBookNew;
	m_talentBook = m_talentBookNew;
	m_skillBook = m_skillBookNew;
	m_buddyBook = m_buddyBookNew;
	m_buddyMaps = m_buddyMapsNew;
	m_gameInitLoaded = true;
	SaveGameInit();
}

std::string Game::TestLuck(short &luckCoin, short &itemId)
{
	int i = 0; 
	for ( ; true; i++)
	{
		if ( i >= m_itemBook.size() ) i = 0;
		if ( 50 != m_itemBook[i].coin 
			&& 100 != m_itemBook[i].coin 
			&& 200 != m_itemBook[i].coin 
			) continue;
		if ( 0 == rand() % 2 ) continue;
		if ( luckCoin + m_itemBook[i].coin > 1000 ) continue;
		itemId = m_itemBook[i].id;
		luckCoin += m_itemBook[i].coin;
		break;
	}
	char result[256];
	sprintf( result, "摇到物品(%s)正能量(%d)剩余机会(%d)\n", 
		m_itemBook[i].name.c_str(), m_itemBook[i].coin, 1000 - luckCoin );
	return std::string(result);
}

std::string Game::Devour( short itemId, int count, int &coin )
{
	if ( 0 >= count ) return "参数错误";

	data::ITEM *pItemBook = Item(itemId, m_itemBook);
	if ( NULL == pItemBook ) return "物品不存在";
	coin = pItemBook->coin * count * 0.9;
	return "";
}

std::string Game::Buy( short itemId, int count, int &coin )
{
	if ( 0 >= count ) return "参数错误";

	data::ITEM *pItemBook = Item(itemId, m_itemBook);
	if ( NULL == pItemBook ) return "物品不存在";
	coin = pItemBook->coin * count;

	return "";
}

data::BUDDY* Game::Encounter( int mapId )
{
	time_t curTime = time(NULL);
	int battleTime = rand() % 26 + 5;
	if ( curTime - m_lastBattleTime < battleTime ) return NULL;

	int i = 0;
	for ( i = 0; i < m_buddyMaps.size(); i++ )
	{
		if ( mapId == m_buddyMaps[i].id )
		{
			char *rare = new char[m_buddyMaps[i].buddys.size() * 5];
			int pos = 0;
			int j = 0;
			data::BUDDY *pBuddy;
			for (j = 0; j < m_buddyMaps[i].buddys.size(); j++)
			{
				pBuddy = Buddy(m_buddyMaps[i].buddys[j], m_buddyBook);
				if ( NULL == pBuddy ) return 0;

				memset(&rare[pos], pBuddy->number, pBuddy->rare);
				pos += pBuddy->rare;
			}
			pos = rand()%pos;
			pBuddy = Buddy(rare[pos], m_buddyBook);
			delete[]rare;
			printf( "经过%d秒%s出现\n", (int)(curTime - m_lastBattleTime), pBuddy->name.c_str() );
			m_lastBattleTime = curTime;
			return pBuddy;
		}
	}

	return NULL;
}

int Game::CreateBattle(unsigned int mePlayerId, const std::string &playerName, 
	std::vector<data::PET> &me, unsigned int shePlayerId, 
	const std::string &enemyName, std::vector<data::PET> &she)
{
	static int battleId = 0;
	battleId++;
	if ( !m_battles[battleId].Init(this, battleId, playerName, enemyName, mePlayerId, shePlayerId, me, she) )
	{
		battleId--;
		return 0;
	}
	return battleId;
}

int Game::CreateBattle(unsigned int mePlayerId, 
	const std::string &playerName, std::vector<data::PET> &me)
{
	unsigned int shePlayerId = 0; 
	const std::string enemyName = "叶茹瑶";
	std::vector<data::PET> she;
	data::PET pet;
	int id = 1;

	pet = m_buddyWorld.Buddy001();
	pet.id = id++;
	she.push_back(pet);
	pet = m_buddyWorld.Buddy002();
	pet.id = id++;
	she.push_back(pet);
	pet = m_buddyWorld.Buddy003();
	pet.id = id++;
	she.push_back(pet);
	pet = m_buddyWorld.Buddy006();
	pet.id = id++;
	she.push_back(pet);
	pet = m_buddyWorld.Buddy008();
	pet.id = id++;
	she.push_back(pet);
	pet = m_buddyWorld.Buddy023();
	pet.id = id++;
	she.push_back(pet);

	return CreateBattle(mePlayerId, playerName, me, shePlayerId, enemyName, she);
}

const char* Game::CheckReady(int battleId, bool me, Battle::Action act, short objectId, Battle::RAND_PARAM &rp)
{
	if ( m_battles.end() == m_battles.find(battleId) ) return "战斗不存在";
	Battle &pBattle = m_battles[battleId];
	return pBattle.CheckReady(me, act, objectId, rp);
}

bool Game::Log( int battleId, std::vector<std::string> &log )
{
	if ( m_battles.end() == m_battles.find(battleId) ) return false;
	Battle &pBattle = m_battles[battleId];
	return pBattle.Log(log);
}

bool Game::Ready(int battleId, bool me, Battle::Action act, short objectId, Battle::RAND_PARAM &rp)
{
	if ( m_battles.end() == m_battles.find(battleId) ) return false;
	Battle &pBattle = m_battles[battleId];
	pBattle.Ready(me, act, objectId, rp);

	return true;
}

const char* Game::ChangePet(int battleId, bool me, short petId)
{
	if ( m_battles.end() == m_battles.find(battleId) ) return "战斗不存在";
	Battle &pBattle = m_battles[battleId];
	return pBattle.ChangePet(me, petId);
}

Battle::WARRIOR* Game::Fighter(int battleId, bool me)
{
	if ( m_battles.end() == m_battles.find(battleId) ) return NULL;
	Battle &pBattle = m_battles[battleId];
	return pBattle.Player(me);
}

