#ifndef GET_USER_DATA_H
#define GET_USER_DATA_H

#include "../../Buffer.h"
#include "../../Protocl.h"

namespace msg
{

/*
ȡ����б�
*/
class GetUserData : public Buffer
{
public:
	//�������
	unsigned int	m_userId;//�û�Id
	//��Ӧ����

public:
	GetUserData();
	virtual ~GetUserData();
	bool Build(bool isResult = false);//���챨�ģ�����
	bool Parse();//�������ģ�����

private:
	unsigned char m_buffer[MAX_MSG_SIZE];
};


}


#endif //GET_USER_DATA_H
