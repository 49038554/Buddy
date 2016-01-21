#include "UseItem.h"

namespace msg
{

UseItem::UseItem()
{
	BindSetting(m_buffer, MAX_MSG_SIZE, BYTE_ORDER, Moudle::Game);
}

UseItem::~UseItem()
{
}

bool UseItem::Build(bool isResult)
{
	SetId(MsgId::useItem, isResult);
	if ( !FillTransmitParam() ) return false;//Tcp服务填写参数
	//请求参数
	if ( !AddData(m_itemId) ) return false;
	if ( !AddData(m_objectId) ) return false;

	//回应参数

	return true;
}

bool UseItem::Parse()
{
	if ( !Buffer::Parse() ) return false;
	//请求参数
	if ( !GetData(m_itemId) ) return false;
	if ( !GetData(m_objectId) ) return false;

	//回应参数

	return true;
}

}
