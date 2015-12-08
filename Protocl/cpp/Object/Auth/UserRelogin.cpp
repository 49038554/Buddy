#include "UserRelogin.h"

namespace msg {

UserRelogin::UserRelogin(void)
{
	BindSetting(m_buffer, MAX_MSG_SIZE, BYTE_ORDER, Moudle::Auth);
	m_position = "位置";
}

UserRelogin::~UserRelogin(void)
{
}

bool UserRelogin::Build(bool isResult)
{
	SetId(MsgId::userRelogin, isResult);
	if (!FillTransmitParam()) return false; // Tcp服务填写参数

	// 请求参数

	// 回应参数
	if (!isResult || ResultCode::Success != m_code) return true;

	if (!AddData(m_position)) return false;  // 登录位置

	return true;
}

bool UserRelogin::Parse(void)
{
	if (!Buffer::Parse()) return false;

	//请求参数

	// 回应参数
	if (!IsResult() || ResultCode::Success != m_code) return true;

	if (!GetData(m_position)) return false;  // 登录位置

	return true;
}

}