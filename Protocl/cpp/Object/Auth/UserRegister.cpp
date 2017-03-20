#include "UserRegister.h"

namespace msg {

UserRegister::UserRegister(void)
{
	BindSetting(m_buffer, MAX_MSG_SIZE, BYTE_ORDER, Moudle::Auth);
}

UserRegister::~UserRegister(void)
{
}

bool UserRegister::Build(bool isResult)
{
	SetId(MsgId::userRegister, isResult);
	if (!FillTransmitParam()) return false; // Tcp������д����

	// �������
	if (!AddData((char)m_accountType)) return false;  // �˺����ͣ�1��ע��/�ֻ�ע��
	if (!AddData(m_account)) return false;            // �˺�:imei/�ֻ���
	if (!AddData(m_pwd)) return false;                //����md5(����)
	
	// ��Ӧ����
	if (!isResult || ResultCode::success != m_code) return true;

	if (!AddData(m_userId)) return false;  // �û�id(ʧ��ʱ����д)

	return true;
}

bool UserRegister::Parse(void)
{
	if (!Buffer::Parse()) return false;

	// �������
	char val;
	if (!GetData(val)) return false; m_accountType = (AccountType::AccountType)val;
	if (!GetData(m_account)) return false;
	if (!GetData(m_pwd)) return false;    

	// ��Ӧ����
	if (!IsResult() || ResultCode::success != m_code) return true;

	if (!GetData(m_userId)) return false;

	return true;
}

}