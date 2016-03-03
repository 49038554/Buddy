#include "ItemBook.h"

namespace msg
{

ItemBook::ItemBook()
{
	BindSetting(m_buffer, MAX_MSG_SIZE, BYTE_ORDER, Moudle::Game);
}

ItemBook::~ItemBook()
{
}

bool ItemBook::Build()
{
	unsigned char count = m_items.size();
	if ( 0 >= count || count > 50 ) 
	{
		return false;
	}
	SetId(MsgId::itemBook, true);
	if ( !FillTransmitParam() ) 
	{
		return false;//Tcp������д����
	}

	//��Ӧ����
	if ( !AddData(count) ) 
	{
		return false;
	}
	int i = 0;
	for ( ; i < count; i++ )
	{
		if ( !AddData(m_items[i].id) ) 
		{
			return false;
		}
		if ( !AddData(m_items[i].name) ) 
		{
			return false;
		}
		if ( !AddData(m_items[i].coin) ) 
		{
			return false;
		}
		if ( !AddData(m_items[i].descript) ) 
		{
			return false;
		}
		if ( !AddData((char)m_items[i].effects.size()) ) //��Ч����
		{
			return false;
		}
		int j = 0;
		for ( j = 0; j < m_items[i].effects.size(); j++ )
		{
			if ( !AddData(m_items[i].effects[j].id) ) 
			{
				return false;
			}
			if ( !AddData(m_items[i].effects[j].step) ) 
			{
				return false;
			}
			if ( !AddData(m_items[i].effects[j].probability) ) 
			{
				return false;
			}
			if ( !AddData(m_items[i].effects[j].agent) ) 
			{
				return false;
			}
		}
	}


	return true;
}

bool ItemBook::Parse()
{
	if ( !Buffer::Parse() ) return false;
	//��Ӧ����
	unsigned char count;
	if ( !GetData(count) ) return false;
	if ( 0 >= count || count > 50 ) return false;

	int i = 0;
	data::ITEM info;
	data::EFFECT effect;
	m_items.clear();
	char val8;
	for ( ; i < count; i++ )
	{
		if ( !GetData(info.id) ) return false;
		if ( !GetData(info.name) ) return false;
		if ( !GetData(info.coin) ) return false;
		if ( !GetData(info.descript) ) return false;
		if ( !GetData(val8) ) return false;//��Ч����
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
		m_items.push_back(info);
	}

	return true;
}

}
