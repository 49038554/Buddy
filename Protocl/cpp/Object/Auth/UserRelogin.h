#ifndef __USER_RELOGIN_H__
#define __USER_RELOGIN_H__

#include "../../Buffer.h"
#include <string>

namespace msg {

// ��¼
class UserRelogin : public Buffer
{
public:
	// �������

	// ��Ӧ����
	std::string		m_position;//��¼λ��

public:
	UserRelogin(void);
	virtual ~UserRelogin(void);
	bool Build(bool isResult = false);  // ���챨�ģ�isResult=true�����Ӧ���ģ�����
	bool Parse(void);                   // ��������������

private:
	unsigned char m_buffer[MAX_MSG_SIZE];
};

}

#endif // __USER_RELOGIN_H__