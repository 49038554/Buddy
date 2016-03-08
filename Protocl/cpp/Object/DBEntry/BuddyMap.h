#ifndef BUDDYMAP_H
#define BUDDYMAP_H

#include "../../Buffer.h"
#include "../../Protocl.h"
#include <vector>

namespace msg
{

//巴迪分布图
class BuddyMap : public Buffer
{
public:
	//回应参数
	std::vector<data::BUDDY_MAP>	m_buddyMaps;//最大30

public:
	BuddyMap();
	virtual ~BuddyMap();
	bool Build();//构造报文，发送
	bool Parse();//解析报文，接收

private:
	unsigned char m_buffer[MAX_MSG_SIZE];
};

}


#endif //BUDDYMAP_H
