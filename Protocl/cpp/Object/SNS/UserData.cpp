#include "UserData.h"


namespace msg
{

UserData::UserData()
{
	BindSetting(m_buffer, MAX_MSG_SIZE, BYTE_ORDER, Moudle::SNS);
	m_userId = 0;
}

UserData::~UserData()
{
}

bool UserData::Build()
{
	SetId(MsgId::userData, true);
	if ( !FillTransmitParam() ) return false;//Tcp服务填写参数
	//请求参数
	
	if ( !AddData(m_userId) ) return false;//用户Id
	if ( !AddData(m_nickName) ) return false;//昵称
	if ( !AddData(m_face) ) return false;//头像URL
	if ( !AddData(m_sex) ) return false;//性别true = 男 false = 女
	if ( !AddData(m_coin) ) return false;//虚拟币（由RMB充值产生）
	if ( !AddData(m_bindImei) ) return false;//账号：Imei
	if ( !AddData(m_bindMobile) ) return false;//账号：手机号码

	return true;
}

bool UserData::Parse()
{
	if ( !Buffer::Parse() ) return false;
	//请求参数
	if ( !GetData(m_userId) ) return false;//用户Id
	if ( !GetData(m_nickName) ) return false;//昵称
	if ( !GetData(m_face) ) return false;//头像URL
	if ( !GetData(m_sex) ) return false;//性别true = 男 false = 女
	if ( !GetData(m_coin) ) return false;//虚拟币（由RMB充值产生）
	if ( !GetData(m_bindImei) ) return false;//账号：Imei
	if ( !GetData(m_bindMobile) ) return false;//账号：手机号码

	return true;
}

}
