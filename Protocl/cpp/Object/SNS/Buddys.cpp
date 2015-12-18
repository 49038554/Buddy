#include "Buddys.h"


namespace msg
{

Buddys::Buddys()
{
	BindSetting(m_buffer, MAX_MSG_SIZE, BYTE_ORDER, Moudle::SNS);
}

Buddys::~Buddys()
{
}

bool Buddys::Build()
{
	SetId(MsgId::buddys, true);
	if ( !FillTransmitParam() ) return false;//Tcp服务填写参数
	//请求参数
	if ( !AddData((char)m_type) ) return false;//列表类型
	if ( !AddData(m_userId) ) return false;//用户Id
	if ( !AddData((char)m_buddys.size()) ) return false;//列表元素个数
	int i = 0;
	for ( ;i < m_buddys.size(); i++ )
	{
		if ( !AddData(m_buddys[i].id) ) return false;
		if ( !AddData(m_buddys[i].face) ) return false;
		if ( !AddData(m_buddys[i].nickName) ) return false;
	}
	
	return true;
}

bool Buddys::Parse()
{
	if ( !Buffer::Parse() ) return false;
	//请求参数
	char val;
	if ( !GetData(val) ) return false;//列表类型
	m_type = (ListType)val;
	if ( !GetData(m_userId) ) return false;//用户Id
	unsigned char count;
	if ( !GetData(count) ) return false;//列表元素个数
	int i = 0;
	BUDDY item;
	for ( ;i < count; i++ )
	{
		if ( !GetData(item.id) ) return false;
		if ( !GetData(item.face) ) return false;
		if ( !GetData(item.nickName) ) return false;
		m_buddys.push_back(item);
	}
	

	return true;
}

}
