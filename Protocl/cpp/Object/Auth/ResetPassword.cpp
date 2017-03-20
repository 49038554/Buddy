#include "ResetPassword.h"

namespace msg {

ResetPassword::ResetPassword(void)
{
	BindSetting(m_buffer, MAX_MSG_SIZE, BYTE_ORDER, Moudle::Auth);
}

ResetPassword::~ResetPassword(void)
{
}

bool ResetPassword::Build(bool isResult)
{
	SetId(MsgId::resetPassword, isResult);
	if (!FillTransmitParam()) return false;  // Tcp服务填写参数

	//请求参数
	if (!AddData(m_pwd)) return false;      // 密码:md5(明文)

	//回应参数
	if (!isResult || ResultCode::success != m_code) return true;

	return true;
}

bool ResetPassword::Parse(void)
{
	if (!Buffer::Parse()) return false;

	// 请求参数
	if (!GetData(m_pwd)) return false;      // 密码:md5(明文)

	// 回应参数
	if (!IsResult() || ResultCode::success != m_code) return true;

	return true;
}

}