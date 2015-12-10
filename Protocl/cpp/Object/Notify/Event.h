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
	unsigned int	m_eventId;//�¼�Id Nofity��д
	int			m_senderId;//������Id
	std::string	m_sender;//�������ǳ�
	int64		m_holdTime;//��Чʱ��
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
