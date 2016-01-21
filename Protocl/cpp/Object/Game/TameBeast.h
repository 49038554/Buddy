#ifndef TAMEBEAST_H
#define TAMEBEAST_H

#include "../../Buffer.h"
#include "../../Protocl.h"

namespace msg
{

//ѱ��Ұ��
class TameBeast : public Buffer
{
public:
	//�������
	unsigned int	m_petId;		//����id
	short			m_buddyNo;		//�͵���ȫ�����
	std::string		m_nickname;		//�ǳ�
	std::string		m_longitude;	//����
	std::string		m_latitude;		//γ��
	std::string		m_address;		//��ַ
	//��Ӧ����

public:
	TameBeast();
	virtual ~TameBeast();
	bool Build(bool isResult = false);//���챨�ģ�����
	bool Parse();//�������ģ�����

private:
	unsigned char m_buffer[MAX_MSG_SIZE];
};


}

#endif //TAMEBEAST_H