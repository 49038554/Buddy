#include "BuddyMap.h"

namespace msg
{

BuddyMap::BuddyMap()
{
	BindSetting(m_buffer, MAX_MSG_SIZE, BYTE_ORDER, Moudle::Game);
}

BuddyMap::~BuddyMap()
{
}

bool BuddyMap::Build()
{
	unsigned char count = m_buddyMaps.size();
	if ( 0 >= count || count > 30 ) return false;
	SetId(MsgId::buddyMap, true);
	if ( !FillTransmitParam() ) return false;//Tcp服务填写参数

	//回应参数
	if ( !AddData(count) ) return false;
	int i = 0;
	for ( ; i < count; i++ )
	{
		if ( !AddData(m_buddyMaps[i].id) ) return false;
		if ( !AddData(m_buddyMaps[i].shape) ) return false;
		if ( !AddData(m_buddyMaps[i].x) ) return false;
		if ( !AddData(m_buddyMaps[i].y) ) return false;
		if ( !AddData(m_buddyMaps[i].radius) ) return false;
		if ( !AddData(m_buddyMaps[i].right) ) return false;
		if ( !AddData(m_buddyMaps[i].bottom) ) return false;
		if ( !AddData(m_buddyMaps[i].city ) )return false;
		if ( !AddData(m_buddyMaps[i].spot ) )return false;
		unsigned char buddyCount = m_buddyMaps[i].buddys.size();
		if ( 0 >= buddyCount || buddyCount > 100 ) return false;
		if ( !AddData(buddyCount) ) return false;
		int j = 0;
		for ( ; j < buddyCount; j++ ) if ( !AddData(m_buddyMaps[i].buddys[j]) ) return false;
	}

	return true;
}

bool BuddyMap::Parse()
{
	if ( !Buffer::Parse() ) return false;

	//回应参数
	unsigned char count;
	if ( !GetData(count) ) return false;
	if ( 0 >= count || count > 30 ) return false;

	int i = 0;
	data::BUDDY_MAP bmap;
	m_buddyMaps.clear();
	for ( ; i < count; i++ )
	{
		if ( !GetData(bmap.id) ) return false;
		if ( !GetData(bmap.shape) ) return false;
		if ( !GetData(bmap.x) ) return false;
		if ( !GetData(bmap.y) ) return false;
		if ( !GetData(bmap.radius) ) return false;
		if ( !GetData(bmap.right) ) return false;
		if ( !GetData(bmap.bottom) ) return false;
		if ( !GetData(bmap.city ) )return false;
		if ( !GetData(bmap.spot ) )return false;
		unsigned char buddyCount = 0;
		if ( !GetData(buddyCount) ) return false;
		if ( 0 >= buddyCount || buddyCount > 100 ) return false;
		int j = 0;
		short buddyNo;
		bmap.buddys.clear();
		for ( ; j < buddyCount; j++ ) 
		{
			if ( !GetData(buddyNo) ) return false;
			bmap.buddys.push_back(buddyNo);
		}
		m_buddyMaps.push_back(bmap);
	}

	return true;
}

}
