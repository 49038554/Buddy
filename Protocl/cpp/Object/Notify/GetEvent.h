#pragma once
#include "../../Buffer.h"
#include "../../Protocl.h"

namespace msg
{

//������Ʒ
class GetEvent : public Buffer
{
public:
	//�������

	//��Ӧ����

public:
	GetEvent();
	virtual ~GetEvent();
	bool Build();//���챨�ģ�����
	bool Parse();//�������ģ�����

private:
	unsigned char m_buffer[MAX_MSG_SIZE];
};


}
