#ifndef KILLBEAST_H
#define KILLBEAST_H

#include "../../Buffer.h"
#include "../../Protocl.h"
#include <map>

namespace msg
{

//ɱ��Ұ��
class KillBeast : public Buffer
{
public:
	//�������
	short						m_killPetNo;			//��ɱ�͵���ȫ�����
	std::map<unsigned int, unsigned int>	m_petIds;	//�����͵���Id
	//��Ӧ����

public:
	KillBeast();
	virtual ~KillBeast();
	bool Build(bool isResult = false);//���챨�ģ�����
	bool Parse();//�������ģ�����

private:
	unsigned char m_buffer[MAX_MSG_SIZE];
};


}


#endif //KILLBEAST_H
