#pragma once
#include "../../Buffer.h"
#include "../../Protocl.h"

namespace msg
{

//��ӻ��
class AddBuddy : public Buffer
{
public:
	//�������
	unsigned int	m_buddyId;//���Id
	unsigned int	m_userId;//�û�Id	//SNS������д
	std::string		m_nickName;//�ǳ�	//SNS������д
	char			m_step;//��ǰ����
	//0 == m_step����
	std::string		m_msg;//��Ϣ
	//1 == m_step����
	bool			m_accepted;//����
	//��Ӧ����

public:
	AddBuddy();
	virtual ~AddBuddy();
	bool Build(bool isResult = false);//���챨�ģ�����
	bool Parse();//�������ģ�����

private:
	unsigned char m_buffer[MAX_MSG_SIZE];
};


}
