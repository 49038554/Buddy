#ifndef __USER_REGISTER_H__
#define __USER_REGISTER_H__

#include "../../Buffer.h"
#include <string>

namespace msg {

// ע��
class UserRegister : public Buffer
{
public:
	// �������
	AccountType::AccountType m_accountType;   // �˺����ͣ�1��ע��/�ֻ�ע��
	std::string              m_account;       // �˺�:imei/�ֻ���
	std::string              m_pwd;           // ����md5(����)

	// ��Ӧ����
	unsigned int			m_userId; // �û�id(ʧ��ʱ����д)

public:
	UserRegister(void);
	virtual ~UserRegister(void);
	bool Build(bool isResult = false); //���챨�ģ�isResult=true�����Ӧ���ģ�����
	bool Parse(void);                  //��������������

private:
	unsigned char m_buffer[MAX_MSG_SIZE];
};

}

#endif // __USER_REGISTER_H__