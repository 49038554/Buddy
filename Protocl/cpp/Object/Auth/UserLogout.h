#pragma once
#include "../../Buffer.h"
#include <string>

namespace msg
{

//�ǳ�
class UserLogout : public Buffer
{
public:
	//�������
	ClientType::ClientType		m_clientType;//Client���ͣ���Ӧ����д��

public:
	UserLogout();
	virtual ~UserLogout();
	bool Build();//���챨�ģ�����
	bool Parse();//��������������

private:
	unsigned char m_buffer[MAX_MSG_SIZE];
};

}