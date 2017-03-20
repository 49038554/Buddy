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
	if (!FillTransmitParam()) return false;  // Tcp������д����

	//�������
	if (!AddData(m_pwd)) return false;      // ����:md5(����)

	//��Ӧ����
	if (!isResult || ResultCode::success != m_code) return true;

	return true;
}

bool ResetPassword::Parse(void)
{
	if (!Buffer::Parse()) return false;

	// �������
	if (!GetData(m_pwd)) return false;      // ����:md5(����)

	// ��Ӧ����
	if (!IsResult() || ResultCode::success != m_code) return true;

	return true;
}

}