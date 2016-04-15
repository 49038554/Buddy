#ifndef __USER_LOGIN_H__
#define __USER_LOGIN_H__

#include "../../Buffer.h"
#include <string>

namespace msg {

// ��¼
class UserLogin : public Buffer
{
public:
	// �������
	ClientType::ClientType   m_clientType;  // Client����
	AccountType::AccountType m_accountType; // �˺����ͣ�1��ע��/�ֻ�ע��
	std::string              m_account;     // �˺�:imei/�ֻ���
	std::string              m_pwd;         // ����md5(����)

	// ��Ӧ����
	unsigned int			m_userId;	// �û�id
	std::string				m_nick;		// �ǳ�

public:
	UserLogin(void);
	virtual ~UserLogin(void);
	bool Build(bool isResult = false);  // ���챨�ģ�isResult=true�����Ӧ���ģ�����
	bool Parse(void);                   // ��������������

private:
	unsigned char m_buffer[MAX_MSG_SIZE];
};

}

#endif // __USER_LOGIN_H__