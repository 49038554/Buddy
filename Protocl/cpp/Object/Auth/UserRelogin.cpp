#include "UserRelogin.h"

namespace msg {

UserRelogin::UserRelogin(void)
{
	BindSetting(m_buffer, MAX_MSG_SIZE, BYTE_ORDER, Moudle::Auth);
	m_position = "δ֪";
}

UserRelogin::~UserRelogin(void)
{
}

bool UserRelogin::Build(bool isResult)
{
	SetId(MsgId::userRelogin, isResult);
	if (!FillTransmitParam()) return false; // Tcp������д����

	// �������
	if (!AddData(m_position)) return false;  // ��¼λ��

	// ��Ӧ����
	if (!isResult || ResultCode::Success != m_code) return true;


	return true;
}

bool UserRelogin::Parse(void)
{
	if (!Buffer::Parse()) return false;

	//�������
	if (!GetData(m_position)) return false;  // ��¼λ��

	// ��Ӧ����
	if (!IsResult() || ResultCode::Success != m_code) return true;

	return true;
}

}