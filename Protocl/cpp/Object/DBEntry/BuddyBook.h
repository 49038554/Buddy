#ifndef BUDDYBOOK_H
#define BUDDYBOOK_H

#include "../../Buffer.h"
#include "../../Protocl.h"
#include <vector>
#include <string>
#include <map>

namespace msg
{

//巴迪图鉴
class BuddyBook : public Buffer
{
public:
	//回应参数
	std::vector<data::BUDDY>		m_buddys;//最大20个

public:
	BuddyBook();
	virtual ~BuddyBook();
	bool Build();//构造报文，发送
	bool Parse();//解析报文，接收

private:
	unsigned char m_buffer[MAX_MSG_SIZE];
};


}


#endif //BUDDYBOOK_H
