#include "BuddyBook.h"


namespace msg
{

BuddyBook::BuddyBook()
{
	BindSetting(m_buffer, MAX_MSG_SIZE, BYTE_ORDER, Moudle::DBEntry);
}

BuddyBook::~BuddyBook()
{
}

bool BuddyBook::Build()
{
	unsigned char count = m_buddys.size();
	if ( 0 >= count || count > 20 ) 
	{
		return false;
	}
	SetId(MsgId::buddyBook, true);
	if ( !FillTransmitParam() ) //Tcp服务填写参数
	{
		return false;
	}

	//回应参数
	if ( !AddData(count) ) 
	{
		return false;
	}
	int i = 0;
	for ( ; i < count; i++ )
	{
		if ( !AddData(m_buddys[i].number) ) //全国编号
		{
			return false;
		}
		if ( !AddData(m_buddys[i].name) ) //名字
		{
			return false;
		}
		if ( !AddData(m_buddys[i].descript) ) //描述
		{
			return false;
		}
		if ( !AddData(m_buddys[i].race1) ) //属性1
		{
			return false;
		}
		if ( !AddData(m_buddys[i].race2) ) //属性2
		{
			return false;
		}
		if ( !AddData(m_buddys[i].talent1) ) //特性1
		{
			return false;
		}
		if ( !AddData(m_buddys[i].talent2) ) //特性2
		{
			return false;
		}
		if ( !AddData(m_buddys[i].talent3) ) //梦特性
		{
			return false;
		}
		if ( !AddData(m_buddys[i].itemId) ) //专属物品，0没有专属物品
		{
			return false;
		}
		if ( !AddData(m_buddys[i].hitPoint) ) //血
		{
			return false;
		}
		if ( !AddData(m_buddys[i].physicalA) ) //攻
		{
			return false;
		}
		if ( !AddData(m_buddys[i].physicalD) ) //防
		{
			return false;
		}
		if ( !AddData(m_buddys[i].specialA) ) //特攻
		{
			return false;
		}
		if ( !AddData(m_buddys[i].specialD) ) //特防
		{
			return false;
		}
		if ( !AddData(m_buddys[i].speed) ) //速度
		{
			return false;
		}
		if ( !AddData(m_buddys[i].rare) ) 	//出现率 1~99
		{
			return false;
		}
		if ( !AddData(m_buddys[i].tame) ) 	//驯化率 1~99
		{
			return false;
		}
		//进化信息
		unsigned char count = m_buddys[i].upBuddys.size();
		if ( 0 > count || count > 10 ) 
		{
			return false;
		}
		if ( !AddData(count) ) 
		{
			return false;
		}
		int j = 0;
		for ( j = 0; j < count; j++ )
		{
			if ( !AddData(m_buddys[i].upBuddys[j]) ) 
			{
				return false;
			}
		}

		//技能信息
		unsigned char skillCount = m_buddys[i].skills.size();
		if ( 0 >= skillCount || skillCount > 80 ) 
		{
			return false;
		}
		if ( !AddData(skillCount) ) 
		{
			return false;
		}
		std::map<unsigned short, bool>::iterator it = m_buddys[i].skills.begin();
		for ( ; it != m_buddys[i].skills.end(); it++ ) 
		{
			if ( !AddData(it->first) ) 
			{
				return false;
			}
			if ( !AddData((bool)it->second) ) 
			{
				return false;
			}
		}
	}


	return true;
}

bool BuddyBook::Parse()
{
	if ( !Buffer::Parse() ) return false;
	//回应参数
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
		//进化信息
		unsigned char count;
		if ( !GetData(count) ) return false;
		if ( 0 > count || count > 10 ) return false;
		int j = 0;
		short upMember;
		info.upBuddys.clear();
		for ( j = 0; j < count; j++ )
		{
			if ( !GetData(upMember) ) return false;
			info.upBuddys.push_back(upMember);
		}

		//技能信息
		unsigned char skillCount = 0;
		if ( !GetData(skillCount) ) return false;
		if ( 0 >= skillCount || skillCount > 80 ) return false;
		short skillId;
		bool isInitSkill;
		info.skills.clear();
		for ( j = 0; j < skillCount; j++ ) 
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
