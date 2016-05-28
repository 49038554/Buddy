#ifndef PUTPET_H
#define PUTPET_H

#include "../../Buffer.h"
#include "../../Protocl.h"

namespace msg
{

//�ƶ�����
class PutPet : public Buffer
{
public:
	//�������
	unsigned int	m_petId;			//����id
	bool			m_toHouse;//���ϵ��Ĵ�
	short			m_houseId;//����id��1��ʼ

public:
	PutPet();
	virtual ~PutPet();
	bool Build(bool isResult = false);//���챨�ģ�����
	bool Parse();//�������ģ�����

private:
	unsigned char m_buffer[MAX_MSG_SIZE];
};


}

#endif //PUTPET_H

