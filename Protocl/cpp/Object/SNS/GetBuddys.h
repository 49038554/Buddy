#ifndef GET_BUDDY_H
#define GET_BUDDY_H

#include "../../Buffer.h"
#include "../../Protocl.h"

namespace msg
{

/*
ȡ����б�
*/
class GetBuddys : public Buffer
{
public:
	//�������
	unsigned int	m_userId;//�û�Id	SNS������д
	//��Ӧ����

public:
	GetBuddys();
	virtual ~GetBuddys();
	bool Build(bool isResult = false);//���챨�ģ�����
	bool Parse();//�������ģ�����

private:
	unsigned char m_buffer[MAX_MSG_SIZE];
};


}


#endif //GET_BUDDY_H
