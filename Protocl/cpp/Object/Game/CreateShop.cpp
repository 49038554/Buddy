#include "CreateShop.h"

namespace msg
{

CreateShop::CreateShop()
{
	BindSetting(m_buffer, MAX_MSG_SIZE, BYTE_ORDER, Moudle::Game);
}

CreateShop::~CreateShop()
{
}

bool CreateShop::Build(bool isResult)
{
	SetId(MsgId::createShop, isResult);
	if ( !FillTransmitParam() ) return false;//Tcp������д����
	//�������
	if ( !AddData(m_longitude) ) return false;
	if ( !AddData(m_latitude) ) return false;
	if ( !AddData(m_address) ) return false;

	//��Ӧ����

	return true;
}

bool CreateShop::Parse()
{
	if ( !Buffer::Parse() ) return false;
	//�������
	if ( !GetData(m_longitude) ) return false;
	if ( !GetData(m_latitude) ) return false;
	if ( !GetData(m_address) ) return false;

	//��Ӧ����

	return true;
}

}
