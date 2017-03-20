#include "BindingPhone.h"

namespace msg {

BindingPhone::BindingPhone(void)
{
	BindSetting(m_buffer, MAX_MSG_SIZE, BYTE_ORDER, Moudle::Auth);
}

BindingPhone::~BindingPhone(void)
{
}

bool BindingPhone::Build(bool isResult)
{
	SetId(MsgId::bindingPhone, isResult);
	if (!FillTransmitParam()) return false;  // Tcp服务填写参数

	//请求参数
	if (!AddData(m_phone)) return false;    // 手机号

	//回应参数
	if (!isResult || ResultCode::success != m_code) return true;

	return true;
}

bool BindingPhone::Parse(void)
{
	if (!Buffer::Parse()) return false;

	// 请求参数
	if (!GetData(m_phone)) return false;    // 手机号

	// 回应参数
	if (!IsResult() || ResultCode::success != m_code) return true;

	return true;
}

}