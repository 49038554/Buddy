#include "BuddyBook.h"


namespace msg
{

BuddyBook::BuddyBook()
{
	BindSetting(m_buffer, MAX_MSG_SIZE, BYTE_ORDER, Moudle::Game);
}

BuddyBook::~BuddyBook()
{
}

bool BuddyBook::Build()
{
	unsigned char count = m_buddys.size();
	if ( 0 >= count || count > 20 ) return false;
	SetId(MsgId::buddyBook, true);
	if ( !FillTransmitParam() ) return false;//Tcp服务填写参数

	//回应参数
	if ( !AddData(m_buddyVersion) ) return false;
	if ( !AddData(count) ) return false;
	int i = 0;
	for ( ; i < count; i++ )
	{
		if ( !AddData(m_buddys[i].number) ) return false;//全国编号
		if ( !AddData(m_buddys[i].name) ) return false;//名字
		if ( !AddData(m_buddys[i].descript) ) return false;//描述
		if ( !AddData(m_buddys[i].race1) ) return false;//属性1
		if ( !AddData(m_buddys[i].race2) ) return false;//属性2
		if ( !AddData(m_buddys[i].talent1) ) return false;//特性1
		if ( !AddData(m_buddys[i].talent2) ) return false;//特性2
		if ( !AddData(m_buddys[i].talent3) ) return false;//梦特性
		if ( !AddData(m_buddys[i].itemId) ) return false;//专属物品，0没有专属物品
		if ( !AddData(m_buddys[i].hitPoint) ) return false;//血
		if ( !AddData(m_buddys[i].physicalA) ) return false;//攻
		if ( !AddData(m_buddys[i].physicalD) ) return false;//防
		if ( !AddData(m_buddys[i].specialA) ) return false;//特攻
		if ( !AddData(m_buddys[i].specialD) ) return false;//特防
		if ( !AddData(m_buddys[i].speed) ) return false;//速度
		if ( !AddData(m_buddys[i].rare) ) return false;	//出现率 1~99
		if ( !AddData(m_buddys[i].tame) ) return false;	//驯化率 1~99
		if ( !AddData(m_buddys[i].upMumber) ) return false;//进化后编号，0无进化态

		unsigned char skillCount = m_buddys[i].skills.size();
		if ( 0 >= skillCount || skillCount > 30 ) return false;
		if ( !AddData(skillCount) ) return false;
		std::map<unsigned short, bool>::iterator it = m_buddys[i].skills.begin();
		for ( ; it != m_buddys[i].skills.end(); it++ ) 
		{
			if ( !AddData(it->first) ) return false;
			if ( !AddData((bool)it->second) ) return false;
		}
	}


	return true;
}

bool BuddyBook::Parse()
{
	if ( !Buffer::Parse() ) return false;
	//回应参数
	if ( !GetData(m_buddyVersion) ) return false;
	unsigned char count;
	if ( !GetData(count) ) return false;
	if ( 0 >= count || count > 20 ) return false;

	int i = 0;
	data::BUDDY info;
	m_buddys.clear();
	for ( ; i < count; i++ )
	{
		if ( !GetData(info.number) ) return false;//全国编号
		if ( !GetData(info.name) ) return false;//名字
		if ( !GetData(info.descript) ) return false;//描述
		if ( !GetData(info.race1) ) return false;//属性1
		if ( !GetData(info.race2) ) return false;//属性2
		if ( !GetData(info.talent1) ) return false;//特性1
		if ( !GetData(info.talent2) ) return false;//特性2
		if ( !GetData(info.talent3) ) return false;//梦特性
		if ( !GetData(info.itemId) ) return false;//专属物品，0没有专属物品
		if ( !GetData(info.hitPoint) ) return false;//血
		if ( !GetData(info.physicalA) ) return false;//攻
		if ( !GetData(info.physicalD) ) return false;//防
		if ( !GetData(info.specialA) ) return false;//特攻
		if ( !GetData(info.specialD) ) return false;//特防
		if ( !GetData(info.speed) ) return false;//速度
		if ( !GetData(info.rare) ) return false;	//出现率 1~99
		if ( !GetData(info.tame) ) return false;	//驯化率 1~99
		if ( !GetData(info.upMumber) ) return false;//进化后编号，0无进化态

		unsigned char skillCount = 0;
		if ( !GetData(skillCount) ) return false;
		if ( 0 >= skillCount || skillCount > 30 ) return false;
		int j = 0;
		short skillId;
		info.skills.clear();
		bool isInitSkill;
		for ( ; j < skillCount; j++ ) 
		{
			if ( !GetData(skillId) ) return false;
			if ( !GetData(isInitSkill) ) return false;
			info.skills[skillId] = isInitSkill;
		}
		m_buddys.push_back(info);
	}

	return true;
}

}
