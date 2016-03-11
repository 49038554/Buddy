#include "SyncItem.h"

namespace msg
{

SyncItem::SyncItem()
{
	BindSetting(m_buffer, MAX_MSG_SIZE, BYTE_ORDER, Moudle::DBEntry);
}

SyncItem::~SyncItem()
{
}

bool SyncItem::Build(bool isResult)
{
	SetId(MsgId::syncItem, isResult);
	if ( !FillTransmitParam() ) return false;//Tcp������д����
	//�������
	if ( m_items.size() > 500 ) return false;
	if ( !AddData((short)m_items.size()) ) return false;
	int i = 0;
	for ( i = 0; i < m_items.size(); i++ )
	{
		if ( !AddData(m_items[i].id) ) return false;
		if ( !AddData(m_items[i].count) ) return false;
		if ( !AddData(m_items[i].synced) ) return false;
	}
	
	//��Ӧ����

	return true;
}

bool SyncItem::Parse()
{
	if ( !Buffer::Parse() ) return false;
	//�������
	short count;
	if ( !GetData(count) ) return false;
	if ( count > 500 ) return false;
	data::PLAYER_ITEM info;
	int i = 0;
	for ( i = 0; i < count; i++ )
	{
		if ( !GetData(info.id) ) return false;
		if ( !GetData(info.count) ) return false;
		if ( !GetData(info.synced) ) return false;
		m_items.push_back(info);
	}

	//��Ӧ����

	return true;
}

}
