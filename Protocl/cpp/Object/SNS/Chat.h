#ifndef CHAT_H
#define CHAT_H

#include "../../Buffer.h"
#include "../../Protocl.h"

namespace msg
{

/*
����
*/
class Chat : public Buffer
{
public:
	enum RecvType
	{
		buddy = 0,
		buddys = 1,
		group = 2,
	};
	//�������
	unsigned int	m_senderId;//������Id	SNS������д
	std::string		m_senderName;//�������ǳ�	SNS������д
	std::string		m_senderFace;//������ͷ��	SNS������д
	RecvType		m_recvType;//���շ�����
	unsigned int	m_recverId;//���շ�Id
	std::string		m_talk;//��������
	//��Ӧ����

public:
	Chat();
	virtual ~Chat();
	bool Build(bool isResult = false);//���챨�ģ�����
	bool Parse();//�������ģ�����

private:
	unsigned char m_buffer[MAX_MSG_SIZE];
};


}


#endif //CHAT_H
