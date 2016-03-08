#ifndef SYNCPETS_H
#define SYNCPETS_H

#include "../../Buffer.h"
#include "../../Protocl.h"
#include <map>
#include <vector>

namespace msg
{

//��ӳ���
class SyncPets : public Buffer
{
public:
	//�������
	std::vector<data::PET> m_pets;//���100ֻ����Ӧʱdata::PET.itemId < 0��ʾͬ��ʧ��
	//��Ӧ����
public:
	SyncPets();
	virtual ~SyncPets();
	bool Build(bool isResult = false);//���챨�ģ�����
	bool Parse();//�������ģ�����

private:
	unsigned char m_buffer[MAX_MSG_SIZE];
};

}


#endif //SYNCPETS_H
