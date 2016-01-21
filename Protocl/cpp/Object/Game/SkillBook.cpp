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
	if ( !FillTransmitParam() ) return false;//Tcp������д����

	//��Ӧ����
	if ( !AddData(m_skillVersion) ) return false;
	if ( !AddData(count) ) return false;
	int i = 0;
	for ( ; i < count; i++ )
	{
		if ( !AddData(m_skills[i].id) ) return false;
		if ( !AddData(m_skills[i].name) ) return false;
		if ( !AddData(m_skills[i].race) ) return false;//����
		if ( !AddData(m_skills[i].power) ) return false;//����
		if ( !AddData(m_skills[i].isPhysical) ) return false;//����
		if ( !AddData(m_skills[i].hitRate) ) return false;//������
		if ( !AddData(m_skills[i].exEffect) ) return false;//��Ч0~20
		if ( !AddData(m_skills[i].isMapSkill) ) return false;//�ǵ�ͼ����
		if ( !AddData(m_skills[i].priority) ) return false;//���ּ���0~6
		if ( !AddData(m_skills[i].descript) ) return false;
	}


	return true;
}

bool SkillBook::Parse()
{
	if ( !Buffer::Parse() ) return false;
	//��Ӧ����
	if ( !GetData(m_skillVersion) ) return false;
	unsigned char count;
	if ( !GetData(count) ) return false;
	if ( 0 >= count || count > 50 ) return false;

	int i = 0;
	data::SKILL info;
	m_skills.clear();
	for ( ; i < count; i++ )
	{
		if ( !GetData(info.id) ) return false;
		if ( !GetData(info.name) ) return false;
		if ( !GetData(info.race) ) return false;//����
		if ( !GetData(info.power) ) return false;//����
		if ( !GetData(info.isPhysical) ) return false;//����
		if ( !GetData(info.hitRate) ) return false;//������
		if ( !GetData(info.exEffect) ) return false;//��Ч0~20
		if ( !GetData(info.isMapSkill) ) return false;//�ǵ�ͼ����
		if ( !GetData(info.priority) ) return false;//���ּ���0~6
		if ( !GetData(info.descript) ) return false;
		m_skills.push_back(info);
	}

	return true;
}

}
