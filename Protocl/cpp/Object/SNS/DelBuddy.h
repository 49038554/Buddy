#ifndef DEL_BUDDY_H
#define DEL_BUDDY_H

#include "../../Buffer.h"
#include "../../Protocl.h"

namespace msg
{

/*
ɾ�����
*/
class DelBuddy : public Buffer
{
public:
	//�������
	unsigned int	m_userId;//�û�Id	SNS������д
	unsigned int	m_buddyId;//���Id

public:
	DelBuddy();
	virtual ~DelBuddy();
	bool Build(bool isResult = false);//���챨�ģ�����
	bool Parse();//�������ģ�����

private:
	unsigned char m_buffer[MAX_MSG_SIZE];
};


}

#endif //DEL_BUDDY_H

