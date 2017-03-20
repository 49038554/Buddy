#include "UserLogin.h"

namespace msg {

UserLogin::UserLogin(void)
{
	BindSetting(m_buffer, MAX_MSG_SIZE, BYTE_ORDER, Moudle::Auth);
}

UserLogin::~UserLogin(void)
{
}

bool UserLogin::Build(bool isResult)
{
	SetId(MsgId::userLogin, isResult);
	if (!FillTransmitParam()) return false; // Tcp服务填写参数

	// 请求参数
	if (!AddData((char)m_clientType)) return false;  // Client类型
	if (!AddData((char)m_accountType)) return false; // 账号类型：1键注册/手机注册
	if (!AddData(m_account)) return false;           // 账号:imei/手机号
	if (!AddData(m_pwd)) return false;               // 密码md5(明文)

	// 回应参数
	if (!isResult || ResultCode::success != m_code) return true;

	if (!AddData(m_userId)) return false; // 用户id
	if (!AddData(m_nick)) return false; // 昵称

	return true;
}

bool UserLogin::Parse(void)
{
	if (!Buffer::Parse()) return false;

	//请求参数
	char val;
	if (!GetData(val)) return false; m_clientType = (ClientType::ClientType)val;
	if (!GetData(val)) return false; m_accountType = (AccountType::AccountType)val;
	if (!GetData(m_account)) return false;
	if (!GetData(m_pwd)) return false;

	// 回应参数
	if (!IsResult() || ResultCode::success != m_code) return true;

	if (!GetData(m_userId)) return false;
	if (!GetData(m_nick)) return false;

	return true;
}

}