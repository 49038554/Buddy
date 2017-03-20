#include "TalentBook.h"

namespace msg
{

TalentBook::TalentBook()
{
	BindSetting(m_buffer, MAX_MSG_SIZE, BYTE_ORDER, Moudle::DBEntry);
}

TalentBook::~TalentBook()
{
}

bool TalentBook::Build( bool isResult )
{
	unsigned char count = m_talents.size();
	if ( 0 >= count || count > 50 ) 
	{
		return false;
	}
	SetId(MsgId::talentBook, true);
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
		if ( !AddData(m_talents[i].id) ) 
		{
			return false;
		}
		if ( !AddData(m_talents[i].name) ) 
		{
			return false;
		}
		if ( !AddData(m_talents[i].descript) ) 
		{
			return false;
		}
		if ( !AddData((char)m_talents[i].effects.size()) ) //特效数量
		{
			return false;
		}
		int j = 0;
		for ( j = 0; j < m_talents[i].effects.size(); j++ )
		{
			if ( !AddData(m_talents[i].effects[j].id) ) 
			{
				return false;
			}
			if ( !AddData(m_talents[i].effects[j].step) ) 
			{
				return false;
			}
			if ( !AddData(m_talents[i].effects[j].probability) ) 
			{
				return false;
			}
			if ( !AddData(m_talents[i].effects[j].agent) ) 
			{
				return false;
			}
		}
	}


	return true;
}

bool TalentBook::Parse()
{
	if ( !Buffer::Parse() ) return false;
	//回应参数
	unsigned char count;
	if ( !GetData(count) ) return false;
	if ( 0 >= count || count > 50 ) return false;

	int i = 0;
	data::TALENT info;
	data::EFFECT effect;
	m_talents.clear();
	char val8;
	for ( ; i < count; i++ )
	{
		if ( !GetData(info.id) ) return false;
		if ( !GetData(info.name) ) return false;
		if ( !GetData(info.descript) ) return false;
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
		m_talents.push_back(info);
	}

	return true;
}

}
