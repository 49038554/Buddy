#ifndef __BINDING_PHONE_H__
#define __BINDING_PHONE_H__

#include "../../Buffer.h"
#include <string>

namespace msg {

// ���ֻ���
class BindingPhone : public Buffer
{
public:
	// �������
	std::string m_phone;    // �ֻ���

	// ��Ӧ����
	// body

public:
	BindingPhone(void);
	virtual ~BindingPhone(void);
	bool Build(bool isResult = false); // ���챨�ģ�isResult=true�����Ӧ���ģ�����
	bool Parse(void);                  // ��������������

private:
	unsigned char m_buffer[MAX_MSG_SIZE];
};

}

#endif // __BINDING_PHONE_H__