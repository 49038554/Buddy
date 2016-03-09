#include "PlayerItems.h"

namespace msg
{

PlayerItems::PlayerItems()
{
	BindSetting(m_buffer, MAX_MSG_SIZE, BYTE_ORDER, Moudle::DBEntry);
}

PlayerItems::~PlayerItems()
{
}

bool PlayerItems::Build()
{
	if ( m_items.size() > 200 ) 
	{
		return false;
	}
	SetId(MsgId::playerItems, true);
	if ( !FillTransmitParam() ) return false;//Tcp服务填写参数
	//回应参数
	if ( !AddData((unsigned char)m_items.size()) )
	{
		return false;
	}
	int i = 0;
	for ( i = 0; i < m_items.size(); i++ )
	{
		if ( !AddData(m_items[i].id) )
		{
			return false;
		}
		if ( !AddData(m_items[i].count) )
		{
			return false;
		}
	}

	return true;
}

bool PlayerItems::Parse()
{
	if ( !Buffer::Parse() ) return false;
	//回应参数
	unsigned char count;
	if ( !GetData(count) )
	{
		return false;
	}
	if ( count > 200 )
	{
		return false;
	}
	int i = 0;
	data::PLAYER_ITEM item;
	for ( i = 0; i < count; i++ )
	{
		if ( !GetData(item.id) )
		{
			return false;
		}
		if ( !GetData(item.count) )
		{
			return false;
		}
		m_items.push_back(item);
	}

	return true;
}

}
