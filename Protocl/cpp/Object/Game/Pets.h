#ifndef PETS_H
#define PETS_H

#include "../../Buffer.h"
#include "../../Protocl.h"
#include <map>
#include <vector>

namespace msg
{

//��������
class Pets : public Buffer
{
public:
	//��Ӧ����
	std::vector<data::PET> m_pets;//���100ֻ
public:
	Pets();
	virtual ~Pets();
	bool Build();//���챨�ģ�����
	bool Parse();//�������ģ�����

private:
	unsigned char m_buffer[MAX_MSG_SIZE];
};

}


#endif //PETS_H
