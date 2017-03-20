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
	if (!FillTransmitParam()) return false;  // Tcp������д����

	//�������
	if (!AddData(m_phone)) return false;    // �ֻ���

	//��Ӧ����
	if (!isResult || ResultCode::success != m_code) return true;

	return true;
}

bool BindingPhone::Parse(void)
{
	if (!Buffer::Parse()) return false;

	// �������
	if (!GetData(m_phone)) return false;    // �ֻ���

	// ��Ӧ����
	if (!IsResult() || ResultCode::success != m_code) return true;

	return true;
}

}