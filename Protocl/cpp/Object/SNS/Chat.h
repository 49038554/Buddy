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
	//�������
	unsigned int	m_senderId;//������Id	SNS������д
	char			m_chatType;//�������ͣ����ģ�Ⱥ�ģ�...
	unsigned int	m_recverId;//���շ�Id
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
