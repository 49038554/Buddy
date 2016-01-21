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
	if ( 0 >= count || count > 50 ) return false;
	SetId(MsgId::itemBook, true);
	if ( !FillTransmitParam() ) return false;//Tcp服务填写参数

	//回应参数
	if ( !AddData(m_itemVersion) ) return false;
	if ( !AddData(count) ) return false;
	int i = 0;
	for ( ; i < count; i++ )
	{
		if ( !AddData(m_items[i].id) ) return false;
		if ( !AddData(m_items[i].name) ) return false;
		if ( !AddData(m_items[i].coin) ) return false;
		if ( !AddData(m_items[i].descript) ) return false;
	}


	return true;
}

bool ItemBook::Parse()
{
	if ( !Buffer::Parse() ) return false;
	//回应参数
	if ( !GetData(m_itemVersion) ) return false;
	unsigned char count;
	if ( !GetData(count) ) return false;
	if ( 0 >= count || count > 50 ) return false;

	int i = 0;
	data::ITEM info;
	m_items.clear();
	for ( ; i < count; i++ )
	{
		if ( !GetData(info.id) ) return false;
		if ( !GetData(info.name) ) return false;
		if ( !GetData(info.coin) ) return false;
		if ( !GetData(info.descript) ) return false;
		m_items.push_back(info);
	}

	return true;
}

}
