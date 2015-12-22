#include "SetUserData.h"


namespace msg
{

SetUserData::SetUserData()
{
	BindSetting(m_buffer, MAX_MSG_SIZE, BYTE_ORDER, Moudle::SNS);
	m_userId = 0;
	m_pwdOpt = false;			
	m_pwd = "";					
	m_nickNameOpt = false;		
	m_nickName = "";			
	m_faceOpt = false;			
	m_face = "";				
	m_sexOpt = false;			
	m_sex = -1;					
	m_coinOpt = false;			
	m_coin = -1;					
	m_bindImeiOpt = false;		
	m_bindImei = "";			
	m_bindMobileOpt = false;	
	m_bindMobile = "";			
}

SetUserData::~SetUserData()
{
}

bool SetUserData::Build(bool isResult)
{
	SetId(MsgId::setUserData, isResult);
	if ( !FillTransmitParam() ) return false;//Tcp服务填写参数
	//请求参数
	if ( !AddData(m_userId) ) return false;//用户Id	SNS服务填写
	if ( "" != m_pwd ) m_pwdOpt = true;
	if ( !AddData(m_pwdOpt) ) return false;//密码被设置
	if ( m_pwdOpt )	if ( !AddData(m_pwd) ) return false;//密码

	if ( "" != m_nickName ) m_nickNameOpt = true;
	if ( !AddData(m_nickNameOpt) ) return false;//昵称被设置
	if ( m_nickNameOpt )	if ( !AddData(m_nickName) ) return false;//昵称

	if ( "" != m_face ) m_faceOpt = true;
	if ( !AddData(m_faceOpt) ) return false;//头像URL被设置
	if ( m_faceOpt )	if ( !AddData(m_face) ) return false;//头像URL

	if ( -1 != m_sex ) m_sexOpt = true;
	if ( !AddData(m_sexOpt) ) return false;//性别被设置
	if ( m_sexOpt )	if ( !AddData((char)(0 == m_sex?0:1)) ) return false;//性别

	if ( -1 != m_coin ) m_coinOpt = true;
	if ( !AddData(m_coinOpt) ) return false;//虚拟币（由RMB充值产生）被设置
	if ( m_coinOpt )	if ( !AddData(m_coin) ) return false;//虚拟币（由RMB充值产生）

	if ( "" != m_bindImei ) m_bindImeiOpt = true;
	if ( !AddData(m_bindImeiOpt) ) return false;//账号：Imei被设置
	if ( m_bindImeiOpt )	if ( !AddData(m_bindImei) ) return false;//账号：Imei

	if ( "" != m_bindMobile ) m_bindMobileOpt = true;
	if ( !AddData(m_bindMobileOpt) ) return false;//账号：手机号码被设置
	if ( m_bindMobileOpt )	if ( !AddData(m_bindMobile) ) return false;//账号：手机号码

	return true;
}

bool SetUserData::Parse()
{
	if ( !Buffer::Parse() ) return false;
	//请求参数
	if ( !GetData(m_userId) ) return false;//用户Id	SNS服务填写

	if ( !GetData(m_pwdOpt) ) return false;//密码被设置
	if ( m_pwdOpt )	if ( !GetData(m_pwd) ) return false;//密码

	if ( !GetData(m_nickNameOpt) ) return false;//昵称被设置
	if ( m_nickNameOpt )	if ( !GetData(m_nickName) ) return false;//昵称

	if ( !GetData(m_faceOpt) ) return false;//头像URL被设置
	if ( m_faceOpt )	if ( !GetData(m_face) ) return false;//头像URL

	if ( !GetData(m_sexOpt) ) return false;//性别被设置
	if ( m_sexOpt )	if ( !GetData(m_sex) ) return false;//性别

	if ( !GetData(m_coinOpt) ) return false;//虚拟币（由RMB充值产生）被设置
	if ( m_coinOpt )	if ( !GetData(m_coin) ) return false;//虚拟币（由RMB充值产生）

	if ( !GetData(m_bindImeiOpt) ) return false;//账号：Imei被设置
	if ( m_bindImeiOpt )	if ( !GetData(m_bindImei) ) return false;//账号：Imei

	if ( !GetData(m_bindMobileOpt) ) return false;//账号：手机号码被设置
	if ( m_bindMobileOpt )	if ( !GetData(m_bindMobile) ) return false;//账号：手机号码

	return true;
}

}
