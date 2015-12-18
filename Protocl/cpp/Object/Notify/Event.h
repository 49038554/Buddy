#pragma once
#include "../../Buffer.h"
#include "../../Protocl.h"

namespace msg
{

//������Ʒ
class Event : public Buffer
{
public:
	enum RecvType
	{
		user = 0,
		buddys = 1,
		group = 2,
	};
	//�������
	unsigned int	m_eventId;//�¼�Id Nofity��д
	unsigned int	m_senderId;//������Id
	std::string	m_sender;//�������ǳ�
	RecvType	m_recvType;//���������� ҵ���������д
	unsigned int	m_recverId;//������Id  ҵ���������д
	int64		m_holdTime;//��Чʱ�䣬-1���ñ���
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
