#ifndef SENDPET_H
#define SENDPET_H

#include "../../Buffer.h"
#include "../../Protocl.h"
#include <vector>

namespace msg
{

//�ų�����
class SendPet : public Buffer
{
public:
	//�������
	int					m_battleId;		//ս��Id
	short				m_petId;		//����id

	//��Ӧ����

public:
	SendPet();
	virtual ~SendPet();
	bool Build(bool isResult = false);//���챨�ģ�����
	bool Parse();//�������ģ�����

private:
	unsigned char m_buffer[MAX_MSG_SIZE];
};

}


#endif //SENDPET_H
