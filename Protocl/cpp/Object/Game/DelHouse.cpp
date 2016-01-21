#include "DelHouse.h"

namespace msg
{

DelHouse::DelHouse()
{
	BindSetting(m_buffer, MAX_MSG_SIZE, BYTE_ORDER, Moudle::Game);
}

DelHouse::~DelHouse()
{
}

bool DelHouse::Build(bool isResult)
{
	SetId(MsgId::delHouse, isResult);
	if ( !FillTransmitParam() ) return false;//Tcp������д����
	//�������
	if ( !AddData(m_buildId) ) return false;
	
	//��Ӧ����

	return true;
}

bool DelHouse::Parse()
{
	if ( !Buffer::Parse() ) return false;
	//�������
	if ( !GetData(m_buildId) ) return false;
	
	//��Ӧ����

	return true;
}

}
