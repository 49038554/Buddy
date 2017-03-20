#include "UserLogout.h"

namespace msg
{

UserLogout::UserLogout()
{
	BindSetting(m_buffer, MAX_MSG_SIZE, BYTE_ORDER, Moudle::Auth);
}

UserLogout::~UserLogout()
{
}

bool UserLogout::Build()
{
	SetId(MsgId::userLogout, false);
	if ( !FillTransmitParam() ) return false;//Tcp������д����
	// �������
	AddData((char)m_clientType);//Client���ͣ���Ӧ����д��

	return true;
}

bool UserLogout::Parse()
{
	if ( !Buffer::Parse() ) return false;

	// �������
	char val;
	if ( !GetData(val) ) return false; m_clientType  = (ClientType::ClientType)val;

	//��Ӧ����
	if ( !IsResult() || ResultCode::success != m_code ) return true;

	return true;
}

}