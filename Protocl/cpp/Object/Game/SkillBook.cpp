#include "SkillBook.h"

namespace msg
{

SkillBook::SkillBook()
{
	BindSetting(m_buffer, MAX_MSG_SIZE, BYTE_ORDER, Moudle::Game);
}

SkillBook::~SkillBook()
{
}

bool SkillBook::Build()
{
	unsigned char count = m_skills.size();
	if ( 0 >= count || count > 50 ) return false;
	SetId(MsgId::skillBook, true);
	if ( !FillTransmitParam() ) return false;//Tcp服务填写参数

	//回应参数
	if ( !AddData(m_skillVersion) ) return false;
	if ( !AddData(count) ) return false;
	int i = 0;
	for ( ; i < count; i++ )
	{
		if ( !AddData(m_skills[i].id) ) return false;
		if ( !AddData(m_skills[i].name) ) return false;
		if ( !AddData(m_skills[i].race) ) return false;//属性
		if ( !AddData(m_skills[i].power) ) return false;//威力
		if ( !AddData(m_skills[i].type) ) return false;//物理
		if ( !AddData(m_skills[i].hitRate) ) return false;//命中率
		if ( m_skills[i].effects.size() > 5 ) return false;
		if ( !AddData((char)m_skills[i].effects.size()) ) return false;//特效数量
		int j = 0;
		for ( j = 0; j < m_skills[i].effects.size(); j++ )
		{
			if ( !AddData((char)(m_skills[i].effects[j])) ) return false;
		}
		if ( !AddData(m_skills[i].isMapSkill) ) return false;//是地图技能
		if ( !AddData(m_skills[i].priority) ) return false;//先手级别0~6
		if ( !AddData(m_skills[i].descript) ) return false;
	}


	return true;
}

bool SkillBook::Parse()
{
	if ( !Buffer::Parse() ) return false;
	//回应参数
	if ( !GetData(m_skillVersion) ) return false;
	unsigned char count;
	if ( !GetData(count) ) return false;
	if ( 0 >= count || count > 50 ) return false;

	int i = 0;
	data::SKILL info;
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
		for ( j = 0; j < val8; j++ )
		{
			if ( !GetData(val8) ) return false;
			m_skills[i].effects.push_back(val8);
		}

		if ( !GetData(info.isMapSkill) ) return false;//是地图技能
		if ( !GetData(info.priority) ) return false;//先手级别0~6
		if ( !GetData(info.descript) ) return false;
		m_skills.push_back(info);
	}

	return true;
}

}
