#ifndef BUDDYBOOK_H
#define BUDDYBOOK_H

#include "../../Buffer.h"
#include "../../Protocl.h"
#include <vector>
#include <string>
#include <map>

namespace msg
{

//�͵�ͼ��
class BuddyBook : public Buffer
{
public:
	//��Ӧ����
	std::vector<data::BUDDY>		m_buddys;//���20��

public:
	BuddyBook();
	virtual ~BuddyBook();
	bool Build();//���챨�ģ�����
	bool Parse();//�������ģ�����

private:
	unsigned char m_buffer[MAX_MSG_SIZE];
};


}


#endif //BUDDYBOOK_H
