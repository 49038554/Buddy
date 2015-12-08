#pragma once
#include "../../Buffer.h"
#include "../../Protocl.h"

namespace msg
{

//������Ʒ
class Event : public Buffer
{
public:
	//�������
	int			m_senderId;//������Id
	std::string	m_sender;//�������ǳ�
	int			m_senderlevel;//�����ߵȼ�
	Buffer		m_msg;//ԭʼ��Ϣ
	//��Ӧ����

public:
	Event();
	virtual ~Event();
	bool Build();//���챨�ģ�����
	bool Parse();//�������ģ�����

private:
	unsigned char m_buffer[MAX_MSG_SIZE];
};


}
