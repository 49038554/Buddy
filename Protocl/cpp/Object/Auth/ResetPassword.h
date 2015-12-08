#ifndef __RESET_PASSWORD_H__
#define __RESET_PASSWORD_H__

#include "../../Buffer.h"
#include <string>

namespace msg {

// ��������
class ResetPassword : public Buffer
{
public:
	// �������
	std::string m_pwd;      // ����:md5(����)

	// ��Ӧ����
	// body

public:
	ResetPassword(void);
	virtual ~ResetPassword(void);
	bool Build(bool isResult = false); // ���챨�ģ�isResult=true�����Ӧ���ģ�����
	bool Parse(void);                  // ��������������

private:
	unsigned char m_buffer[MAX_MSG_SIZE];
};

}

#endif // __RESET_PASSWORD_H__