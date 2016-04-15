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
	if ( Race::unknow == state ) return "正常";
	if ( Race::pu == state ) return "睡";
	if ( Race::bing == state ) return "冰";
	if ( Race::huo == state ) return "烧";
	if ( Race::dian == state ) return "麻";
	if ( Race::du == state ) return "毒";
	if ( Race::gui == state ) return "乱";
	
	return "正常";
}
