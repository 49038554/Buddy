#pragma once
#include "../Buffer.h"

namespace msg
{

//������֤
class ConnectAuth : public Buffer
{
public:
	//�������
	Moudle::Moudle	m_moudleId;//ģ��Id
	int				m_nodeId;//���Id
	//��Ӧ����

public:
	ConnectAuth();
	virtual ~ConnectAuth();
	bool Build();//���챨�ģ�����
	bool Parse();//�������ģ�����

private:
	unsigned char m_buffer[MAX_MSG_SIZE];
};


}
