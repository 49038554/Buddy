#ifndef BUDDYMAP_H
#define BUDDYMAP_H

#include "../../Buffer.h"
#include "../../Protocl.h"
#include <vector>

namespace msg
{

//�͵Ϸֲ�ͼ
class BuddyMap : public Buffer
{
public:
	//��Ӧ����
	std::vector<data::BUDDY_MAP>	m_buddyMaps;//���30

public:
	BuddyMap();
	virtual ~BuddyMap();
	bool Build();//���챨�ģ�����
	bool Parse();//�������ģ�����

private:
	unsigned char m_buffer[MAX_MSG_SIZE];
};

}


#endif //BUDDYMAP_H
