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
	if (!FillTransmitParam()) return false; // Tcp������д����

	// �������
	if (!AddData((char)m_clientType)) return false;  // Client����
	if (!AddData((char)m_accountType)) return false; // �˺����ͣ�1��ע��/�ֻ�ע��
	if (!AddData(m_account)) return false;           // �˺�:imei/�ֻ���
	if (!AddData(m_pwd)) return false;               // ����md5(����)

	// ��Ӧ����
	if (!isResult || ResultCode::success != m_code) return true;

	if (!AddData(m_userId)) return false; // �û�id
	if (!AddData(m_nick)) return false; // �ǳ�

	return true;
}

bool UserLogin::Parse(void)
{
	if (!Buffer::Parse()) return false;

	//�������
	char val;
	if (!GetData(val)) return false; m_clientType = (ClientType::ClientType)val;
	if (!GetData(val)) return false; m_accountType = (AccountType::AccountType)val;
	if (!GetData(m_account)) return false;
	if (!GetData(m_pwd)) return false;

	// ��Ӧ����
	if (!IsResult() || ResultCode::success != m_code) return true;

	if (!GetData(m_userId)) return false;
	if (!GetData(m_nick)) return false;

	return true;
}

}