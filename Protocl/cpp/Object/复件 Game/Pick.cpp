#include "Pick.h"

namespace msg
{

Pick::Pick()
{
	BindSetting(m_buffer, MAX_MSG_SIZE, BYTE_ORDER, Moudle::Game);
}

Pick::~Pick()
{
}

bool Pick::Build(bool isResult)
{
	if ( m_itemIds.size() > 200 ) return false;

	unsigned char count = m_itemIds.size();
	SetId(MsgId::pick, isResult);
	if ( !FillTransmitParam() ) return false;//Tcp服务填写参数
	//请求参数
	if ( !AddData(m_treeId) ) return false;
	if ( !AddData(count) ) return false;
	int i = 0;
	for ( ; i < count; i++ )
	{
		if ( !AddData((short)m_itemIds[i]) ) return false;
	}

	//回应参数
	if ( !isResult || ResultCode::Success != m_code ) return true;

	return true;
}

bool Pick::Parse()
{
	if ( !Buffer::Parse() ) return false;
	//请求参数
	if ( !GetData(m_treeId) ) return false;
	unsigned char count;
	if ( !GetData(count) ) return false;
	int i = 0;
	short itemId;
	for ( ; i < count; i++ )
	{
		if ( !GetData(itemId) ) return false;
		m_itemIds.push_back(itemId);
	}

	//回应参数
	if ( !IsResult() || ResultCode::Success != m_code ) return true;

	return true;
}

}
