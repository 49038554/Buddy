#include "SkillBook.h"

namespace msg
{

SkillBook::SkillBook()
{
	BindSetting(m_buffer, MAX_MSG_SIZE, BYTE_ORDER, Moudle::DBEntry);
}

SkillBook::~SkillBook()
{
}

bool SkillBook::Build( bool isResult )
{
	unsigned char count = m_skills.size();
	if ( 0 >= count || count > 50 ) 
	{
		return false;
	}
	SetId(MsgId::skillBook, true);
	if ( !FillTransmitParam() ) 
	{
		return false;//Tcp服务填写参数
	}

	//回应参数
	if ( !AddData(count) ) 
	{
		return false;
	}
	int i = 0;
	for ( ; i < count; i++ )
	{
		if ( !AddData(m_skills[i].id) ) 
		{
			return false;
		}
		if ( !AddData(m_skills[i].name) )
		{
			return false;
		}
		if ( !AddData(m_skills[i].race) ) //属性
		{
			return false;
		}
		if ( !AddData(m_skills[i].power) ) //威力
		{
			return false;
		}
		if ( !AddData(m_skills[i].type) ) //物理
		{
			return false;
		}
		if ( !AddData(m_skills[i].hitRate) ) //命中率
		{
			return false;
		}
		if ( m_skills[i].effects.size() > 5 ) 
		{
			return false;
		}
		if ( !AddData((char)m_skills[i].effects.size()) ) //特效数量
		{
			return false;
		}
		int j = 0;
		for ( j = 0; j < m_skills[i].effects.size(); j++ )
		{
			if ( !AddData(m_skills[i].effects[j].id) ) 
			{
				return false;
			}
			if ( !AddData(m_skills[i].effects[j].step) ) 
			{
				return false;
			}
			if ( !AddData(m_skills[i].effects[j].probability) ) 
			{
				return false;
			}
			if ( !AddData(m_skills[i].effects[j].agent) ) 
			{
				return false;
			}
		}
		if ( !AddData(m_skills[i].isMapSkill) ) //是地图技能
		{
			return false;
		}
		if ( !AddData(m_skills[i].descript) ) 
		{
			return false;
		}
	}


	return true;
}

bool SkillBook::Parse()
{
	if ( !Buffer::Parse() ) return false;
	//回应参数
	unsigned char count;
	if ( !GetData(count) ) return false;
	if ( 0 >= count || count > 50 ) return false;

	int i = 0;
	data::SKILL info;
	data::EFFECT effect;
	m_skills.clear();
	char val8;
	for ( ; i < count; i++ )
	{
		if ( !GetData(info.id) ) return false;
		if ( !GetData(info.name) ) return false;
		if ( !GetData(info.race) ) return false;//属性
		if ( !GetData(info.power) ) return false;//威力
		if ( !GetData(info.type) ) return false;//物理
		if ( !GetData(info.hitRate) ) return false;//命中率
		if ( !GetData(val8) ) return false;//特效数量
		if ( val8 > 5 ) return false;
		int j = 0;
		info.effects.clear();
		for ( j = 0; j < val8; j++ )
		{
			if ( !GetData(effect.id) ) return false;
			if ( !GetData(effect.step) ) return false;
			if ( !GetData(effect.probability) ) return false;
			if ( !GetData(effect.agent) ) return false;
			info.effects.push_back(effect);
		}

		if ( !GetData(info.isMapSkill) ) return false;//是地图技能
		if ( !GetData(info.descript) ) return false;
		m_skills.push_back(info);
	}

	return true;
}

}
