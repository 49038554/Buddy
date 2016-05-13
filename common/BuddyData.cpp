#include "BuddyData.h"
#include <ctype.h>

data::ITEM* Item(short id, std::vector<data::ITEM> &items)
{
	int i = 0;
	for ( i = 0; i < items.size(); i++ )
	{
		if ( id == items[i].id ) return &items[i];
	}

	return NULL;
}

data::ITEM* Item(const std::string name, std::vector<data::ITEM> &items)
{
	int i = 0;
	for ( i = 0; i < items.size(); i++ )
	{
		if ( name == items[i].name ) return &items[i];
	}

	return NULL;
}

data::TALENT* Talent(short id, std::vector<data::TALENT> &talents)
{
	int i = 0;
	for ( i = 0; i < talents.size(); i++ )
	{
		if ( id == talents[i].id ) return &talents[i];
	}

	return NULL;
}

data::TALENT* Talent(const std::string name, std::vector<data::TALENT> &talents)
{
	int i = 0;
	for ( i = 0; i < talents.size(); i++ )
	{
		if ( name == talents[i].name ) return &talents[i];
	}

	return NULL;
}

data::SKILL* Skill(short id, std::vector<data::SKILL> &skills)
{
	int i = 0;
	for ( i = 0; i < skills.size(); i++ )
	{
		if ( id == skills[i].id ) return &skills[i];
	}

	return NULL;
}

data::SKILL* Skill(const std::string name, std::vector<data::SKILL> &skills)
{
	int i = 0;
	for ( i = 0; i < skills.size(); i++ )
	{
		if ( name == skills[i].name ) return &skills[i];
	}

	return NULL;
}

data::BUDDY* Buddy(short number, std::vector<data::BUDDY> &buddys)
{
	int i = 0;
	for ( i = 0; i < buddys.size(); i++ )
	{
		if ( number == buddys[i].number ) return &buddys[i];
	}

	return NULL;
}

data::BUDDY* Buddy(const std::string name, std::vector<data::BUDDY> &buddys)
{
	int i = 0;
	for ( i = 0; i < buddys.size(); i++ )
	{
		if ( name == buddys[i].name ) return &buddys[i];
	}

	return NULL;
}

data::BUDDY_MAP* BuddyMap(int id, std::vector<data::BUDDY_MAP> &BuddyMaps)
{
	int i = 0;
	for ( i = 0; i < BuddyMaps.size(); i++ )
	{
		if ( id == BuddyMaps[i].id ) return &BuddyMaps[i];
	}

	return NULL;
}

data::PLAYER_ITEM* PlayerItem(short id, std::vector<data::PLAYER_ITEM> &items)
{
	int i = 0;
	for ( i = 0; i < items.size(); i++ )
	{
		if ( id == items[i].id ) return &items[i];
	}

	return NULL;
}

data::PET* Pet(int id, std::vector<data::PET> &pets)
{
	int i = 0;
	for ( i = 0; i < pets.size(); i++ )
	{
		if ( id == pets[i].id ) return &pets[i];
	}

	return NULL;
}

bool StrSame(const std::string &str1, const std::string &str2)
{
	if ( str1.size() != str2.size() ) return false;

	int i = 0;
	for ( i = 0; i < str1.size(); i++ )
	{
		if ( toupper(str1.c_str()[i]) != toupper(str2.c_str()[i]) ) return false;
	}

	return true;
}

int GetNature(const std::string &add, const std::string &dec)
{
	if (StrSame("WG", add) && StrSame("WF", dec) ) return 1;
	if (StrSame("WG", add) && StrSame("TG", dec) ) return 2;
	if (StrSame("WG", add) && StrSame("TF", dec) ) return 3;
	if (StrSame("WG", add) && StrSame("SD", dec) ) return 4;

	if (StrSame("WF", add) && StrSame("WG", dec) ) return 5;
	if (StrSame("WF", add) && StrSame("TG", dec) ) return 6;
	if (StrSame("WF", add) && StrSame("TF", dec) ) return 7;
	if (StrSame("WF", add) && StrSame("SD", dec) ) return 8;

	if (StrSame("TG", add) && StrSame("WG", dec) ) return 9;
	if (StrSame("TG", add) && StrSame("WF", dec) ) return 10;
	if (StrSame("TG", add) && StrSame("TF", dec) ) return 11;
	if (StrSame("TG", add) && StrSame("SD", dec) ) return 12;

	if (StrSame("TF", add) && StrSame("WG", dec) ) return 13;
	if (StrSame("TF", add) && StrSame("WF", dec) ) return 14;
	if (StrSame("TF", add) && StrSame("TG", dec) ) return 15;
	if (StrSame("TF", add) && StrSame("SD", dec) ) return 16;

	if (StrSame("SD", add) && StrSame("WG", dec) ) return 17;
	if (StrSame("SD", add) && StrSame("WF", dec) ) return 18;
	if (StrSame("SD", add) && StrSame("TG", dec) ) return 19;
	if (StrSame("SD", add) && StrSame("TF", dec) ) return 20;

	return 0;
}

double GetNatureCal(int nature, const std::string &attr)
{
	if ( 0 == nature ) return 1;

	if ("WG" == attr && 1 == nature ) return 1.1;
	if ("WG" == attr && 2 == nature ) return 1.1;
	if ("WG" == attr && 3 == nature ) return 1.1;
	if ("WG" == attr && 4 == nature ) return 1.1;
	if (1 == nature && "WF" == attr ) return 1/1.1;
	if (2 == nature && "TG" == attr ) return 1/1.1;
	if (3 == nature && "TF" == attr ) return 1/1.1;
	if (4 == nature && "SD" == attr ) return 1/1.1;

	if ("WF" == attr && 5 == nature ) return 1.1;
	if ("WF" == attr && 6 == nature ) return 1.1;
	if ("WF" == attr && 7 == nature ) return 1.1;
	if ("WF" == attr && 8 == nature ) return 1.1;
	if (5 == nature && "WG" == attr ) return 1/1.1;
	if (6 == nature && "TG" == attr ) return 1/1.1;
	if (7 == nature && "TF" == attr ) return 1/1.1;
	if (8 == nature && "SD" == attr ) return 1/1.1;

	if ("TG" == attr && 9  == nature ) return 1.1;
	if ("TG" == attr && 10 == nature ) return 1.1;
	if ("TG" == attr && 11 == nature ) return 1.1;
	if ("TG" == attr && 12 == nature ) return 1.1;
	if (9  == nature && "WG" == attr ) return 1/1.1;
	if (10 == nature && "WF" == attr ) return 1/1.1;
	if (11 == nature && "TF" == attr ) return 1/1.1;
	if (12 == nature && "SD" == attr ) return 1/1.1;

	if ("TF" == attr && 13 == nature ) return 1.1;
	if ("TF" == attr && 14 == nature ) return 1.1;
	if ("TF" == attr && 15 == nature ) return 1.1;
	if ("TF" == attr && 16 == nature ) return 1.1;
	if (13 == nature && "WG" == attr ) return 1/1.1;
	if (14 == nature && "WF" == attr ) return 1/1.1;
	if (15 == nature && "TG" == attr ) return 1/1.1;
	if (16 == nature && "SD" == attr ) return 1/1.1;

	if ("SD" == attr && 17 == nature ) return 1.1;
	if ("SD" == attr && 18 == nature ) return 1.1;
	if ("SD" == attr && 19 == nature ) return 1.1;
	if ("SD" == attr && 20 == nature ) return 1.1;
	if (17 == nature && "WG" == attr ) return 1/1.1;
	if (18 == nature && "WF" == attr ) return 1/1.1;
	if (19 == nature && "TG" == attr ) return 1/1.1;
	if (20 == nature && "TF" == attr ) return 1/1.1;

	return 1;
}

char* StateDes( char state )
{
	if ( Race::unknow == state ) return "Õý³£";
	if ( Race::pu == state ) return "Ë¯";
	if ( Race::bing == state ) return "±ù";
	if ( Race::huo == state ) return "ÉÕ";
	if ( Race::dian == state ) return "Âé";
	if ( Race::du == state ) return "¶¾";
	if ( Race::gui == state ) return "ÂÒ";
	
	return "Õý³£";
}

bool SavePets(mdk::File &db, std::vector<data::PET> &pets, std::vector<data::BUDDY> &buddyBook)
{
	data::PET *pInfo;
	int count = pets.size();
	if ( count <= 0 ) return false;

	db.Write(&count, sizeof(int));
	int i = 0; 
	char varChar;
	for ( i = 0; i < pets.size(); i++ )
	{
		pInfo = &pets[i];
		db.Write(&pInfo->id, sizeof(int));
		db.Write(&pInfo->number, sizeof(short));
		db.Write(&pInfo->talent, sizeof(char));
		db.Write(&pInfo->nature, sizeof(char));
		db.Write(&pInfo->HP, sizeof(short));
		db.Write(&pInfo->WG, sizeof(short));
		db.Write(&pInfo->WF, sizeof(short));
		db.Write(&pInfo->TG, sizeof(short));
		db.Write(&pInfo->TF, sizeof(short));
		db.Write(&pInfo->SD, sizeof(short));
		db.Write(&pInfo->SDHealthy, sizeof(char));
		db.Write(&pInfo->HPMuscle, sizeof(char));
		db.Write(&pInfo->WGMuscle, sizeof(char));
		db.Write(&pInfo->WFMuscle, sizeof(char));
		db.Write(&pInfo->TGMuscle, sizeof(char));
		db.Write(&pInfo->TFMuscle, sizeof(char));
		db.Write(&pInfo->SDMuscle, sizeof(char));

		varChar = pInfo->race.size();
		db.Write(&varChar, sizeof(char));
		int j = 0;
		for ( j = 0; j < pInfo->race.size(); j++ )
		{
			varChar = pInfo->race[j];
			db.Write(&varChar, sizeof(char));
		}

		varChar = pInfo->synced?1:0;
		db.Write(&varChar, sizeof(char));
		if ( "" == pInfo->nick )
		{
			data::BUDDY *pBuddy = Buddy(pInfo->number, buddyBook);
			if ( NULL == pBuddy ) pInfo->nick = "Î´Öª°ÍµÏ";
			else pInfo->nick = pBuddy->name;
		}
		db.Write(pInfo->nick, 20);
		db.Write(&pInfo->curHP, sizeof(short));
		db.Write(&pInfo->state, sizeof(char));
		db.Write(&pInfo->skill1, sizeof(short));
		db.Write(&pInfo->skill2, sizeof(short));
		db.Write(&pInfo->skill3, sizeof(short));
		db.Write(&pInfo->skill4, sizeof(short));
		db.Write(&pInfo->itemId, sizeof(short));
	}

	return true;
}

int LoadPets(mdk::File &db, std::vector<data::PET> &pets, std::vector<data::BUDDY> &buddyBook)
{
	data::PET info;
	int count = 0;
	if ( mdk::File::success != db.Read(&count, sizeof(int)) ) return 1;
	if ( count <= 0 ) return 2;

	int i = 0;
	char varChar;
	for ( i = 0; i < count; i++ )
	{
		info.nick = "";
		info.race.clear();
		if ( mdk::File::success != db.Read(&info.id, sizeof(int)) ) return 3;
		if ( mdk::File::success != db.Read(&info.number, sizeof(short)) ) return 4;
		if ( mdk::File::success != db.Read(&info.talent, sizeof(char)) ) return 5;
		if ( mdk::File::success != db.Read(&info.nature, sizeof(char)) ) return 6;
		if ( mdk::File::success != db.Read(&info.HP, sizeof(short)) ) return 12;
		if ( mdk::File::success != db.Read(&info.WG, sizeof(short)) ) return 13;
		if ( mdk::File::success != db.Read(&info.WF, sizeof(short)) ) return 14;
		if ( mdk::File::success != db.Read(&info.TG, sizeof(short)) ) return 15;
		if ( mdk::File::success != db.Read(&info.TF, sizeof(short)) ) return 16;
		if ( mdk::File::success != db.Read(&info.SD, sizeof(short)) ) return 17;
		if ( mdk::File::success != db.Read(&info.SDHealthy, sizeof(char)) ) return 23;
		if ( mdk::File::success != db.Read(&info.HPMuscle, sizeof(char)) ) return 24;
		if ( mdk::File::success != db.Read(&info.WGMuscle, sizeof(char)) ) return 25;
		if ( mdk::File::success != db.Read(&info.WFMuscle, sizeof(char)) ) return 26;
		if ( mdk::File::success != db.Read(&info.TGMuscle, sizeof(char)) ) return 27;
		if ( mdk::File::success != db.Read(&info.TFMuscle, sizeof(char)) ) return 28;
		if ( mdk::File::success != db.Read(&info.SDMuscle, sizeof(char)) ) return 29;

		char len = 0;
		if ( mdk::File::success != db.Read(&len, sizeof(char)) ) return 30;
		if ( len > 17 || len < 0 ) return 31;
		int j = 0;
		for ( j = 0; j < len; j++ )
		{
			if ( mdk::File::success != db.Read(&varChar, sizeof(char)) ) return 32;
			info.race.push_back(varChar);
		}

		if ( mdk::File::success != db.Read(&varChar, sizeof(char)) ) return 33;
		info.synced = (0 == varChar?false:true);
 		if ( mdk::File::success != db.Read(info.nick, 20) ) return 43;
		if ( mdk::File::success != db.Read(&info.curHP, sizeof(short)) ) return 34;
		if ( mdk::File::success != db.Read(&info.state, sizeof(char)) ) return 35;
		if ( mdk::File::success != db.Read(&info.skill1, sizeof(short)) ) return 36;
		if ( mdk::File::success != db.Read(&info.skill2, sizeof(short)) ) return 37;
		if ( mdk::File::success != db.Read(&info.skill3, sizeof(short)) ) return 38;
		if ( mdk::File::success != db.Read(&info.skill4, sizeof(short)) ) return 39;
		if ( mdk::File::success != db.Read(&info.itemId, sizeof(short)) ) return 40;
		if ( "" == info.nick )
		{
			data::BUDDY *pBuddy = Buddy(info.number, buddyBook);
			if ( NULL == pBuddy ) continue;
			info.nick = pBuddy->name;
		}
		pets.push_back(info);
	}

	return 0;
}
