#ifndef GROWUP_H
#define GROWUP_H

#include "../../Buffer.h"
#include "../../Protocl.h"

namespace msg
{

//����
class GrowUp : public Buffer
{
public:
	//�������
	unsigned int	m_petId;			//����id
	//��Ӧ����

public:
	GrowUp();
	virtual ~GrowUp();
	bool Build(bool isResult = false);//���챨�ģ�����
	bool Parse();//�������ģ�����

private:
	unsigned char m_buffer[MAX_MSG_SIZE];
};


}


#endif //GROWUP_H
