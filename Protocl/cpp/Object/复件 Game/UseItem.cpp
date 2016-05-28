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
	if ( !FillTransmitParam() ) return false;//Tcp������д����
	//�������
	if ( !AddData(m_itemId) ) return false;
	if ( !AddData(m_objectId) ) return false;

	//��Ӧ����

	return true;
}

bool UseItem::Parse()
{
	if ( !Buffer::Parse() ) return false;
	//�������
	if ( !GetData(m_itemId) ) return false;
	if ( !GetData(m_objectId) ) return false;

	//��Ӧ����

	return true;
}

}
