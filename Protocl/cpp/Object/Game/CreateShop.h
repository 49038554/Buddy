#ifndef CREATESHOP_H
#define CREATESHOP_H

#include "../../Buffer.h"
#include "../../Protocl.h"
#include <vector>

namespace msg
{

//����
class CreateShop : public Buffer
{
public:
	//�������
	std::string		m_longitude;	//����
	std::string		m_latitude;		//γ��
	std::string		m_address;		//��ַ

	//��Ӧ����

public:
	CreateShop();
	virtual ~CreateShop();
	bool Build(bool isResult = false);//���챨�ģ�����
	bool Parse();//�������ģ�����

private:
	unsigned char m_buffer[MAX_MSG_SIZE];
};

}


#endif //CREATESHOP_H
